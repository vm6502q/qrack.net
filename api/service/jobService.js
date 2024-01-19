// jobService.js

// Data Access Layer
const ModelService = require('./modelService')

// Services
const OutputService = require('./outputService.js')
const outputService = OutputService()

// Database Model
const db = require('../model/index')
const Job = db.job

const Qrack = require('./Qrack.js')
const QrackWASM = require('./Qrack.wasm')

const qrack = Qrack({
  locateFile: () => {
    return QrackWASM
  }
})

class JobService extends ModelService {
  constructor () {
    super(Job)
  }

  async sanitize (job) {
    return {
      id: job.id,
      userId: job.userId,
      status: job.status
    }
  }

  async get (jobId) {
    const job = await this.getByPk(jobId)
    if (!job) {
      return { success: false, error: 'Job ID not found.' }
    }

    return { success: true, body: user }
  }

  async getSanitized (userId) {
    const job = await this.getByPk(jobId)
    if (!job) {
      return { success: false, error: 'Job ID not found.' }
    }
    return { success: true, body: await this.sanitize(job) }
  }

  async validateCreatRequest (reqBody) {
    if (!reqBody.program) {
      return { success: false, message: 'Job creation request must contain the "program" parameter.' }
    }
    if (!Array.isArray(reqBody.program)) {
      return { success: false, message: 'Job creation request "program" parameter must be an array.' }
    }
    return { success: true }
  }

  async invalid_argument_error (job) {
    // Job status 2: FAILURE
    job.jobStatusTypeId = 2
    job.statusMessage =
     'All simulator IDs and quantum neuron IDs should be specified as names in the output space of the job. ' +
     '(Methods that produce output, at all, always save it to the "output space" of the job, to the ' +
     'variable named by the "output" parameter of the job program line that produces output.)'
    await job.save()
  }

  async validate_sid (name, job) {
    const v = await outputService.getByJobIdAndName(job.id, name)
    if (!v) {
      await this.invalid_argument_error(job)
      return null
    }
    switch (v.outputTypeId) {
      case 1:
        return parseInt(v.value)
      default:
        return null
    }
  }

  async create (reqBody, userId) {
    const validationResult = await this.validateCreateRequest(reqBody)
    if (!validationResult.success) {
      return validationResult
    }

    let job = await this.SequelizeServiceInstance.new()
    job.userId = userId
    // Job status 3: RUNNING
    job.jobStatusTypeId = 3

    const result = await this.create(job)
    if (!result.success) {
      return result
    }

    job = result.body
    await job.save()

    let tmp, tmp2, tmpLongVec, tmpLongVec2, tmpCharVec, tmpDoubleVec
    const p = reqBody.program
    qrack.then(async (core) => {
      for (i in p) {
        switch (i.name) {
          case 'init_general':
            await outputService.createOrUpdate(job.id, i.output, core.init_general(...i.parameters), 1)
            break
          case 'init_stabilizer':
            await outputService.createOrUpdate(job.id, i.output, core.init_stabilizer(...i.parameters), 1)
            break
          case 'init_qbdd':
            await outputService.createOrUpdate(job.id, i.output, core.init_qbdd(...i.parameters), 1)
            break
          case 'init_clone':
            tmp = this.validate_sid(i.parameters[0], job)
            if (!tmp) {
              return
            }
            await outputService.createOrUpdate(job.id, i.output, core.init_clone(tmp), 1)
            break
          case 'destroy':
            tmp = this.validate_sid(i.parameters[0], job)
            if (!tmp) {
              return
            }
            core.destroy(tmp)
            break
          case 'seed':
            tmp = this.validate_sid(i.parameters[0], job)
            if (!tmp) {
              return
            }
            i.parameters.shift()
            core.seed(tmp, ...i.parameters)
            break
          case 'try_separate_1qb':
            tmp = this.validate_sid(i.parameters[0], job)
            if (!tmp) {
              return
            }
            i.parameters.shift()
            await outputService.createOrUpdate(job.id, i.output, core.try_separate_1qb(tmp, ...i.parameters), 2)
            break
          case 'try_separate_2qb':
            tmp = this.validate_sid(i.parameters[0], job)
            if (!tmp) {
              return
            }
            i.parameters.shift()
            await outputService.createOrUpdate(job.id, i.output, core.try_separate_2qb(tmp, ...i.parameters), 2)
            break
          case 'try_separate_tol':
            tmp = this.validate_sid(i.parameters[0], job)
            if (!tmp) {
              return
            }
            i.parameters.shift()
            tmpLongVec = core.VectorLong(i.parameters[0])
            i.parameters.shift()
            await outputService.createOrUpdate(job.id, i.output, core.try_separate_tol(tmp, tmpVec, i.parameters[0]), 2)
            break
          case 'get_unitary_fidelity':
            tmp = this.validate_sid(i.parameters[0], job)
            if (!tmp) {
              return
            }
            await outputService.createOrUpdate(job.id, i.output, core.get_unitary_fidelity(tmp), 2)
            break
          case 'reset_unitary_fidelity':
            tmp = this.validate_sid(i.parameters[0], job)
            if (!tmp) {
              return
            }
            core.reset_unitary_fidelity(tmp)
            break
          case 'set_sdrp':
            tmp = this.validate_sid(i.parameters[0], job)
            if (!tmp) {
              return
            }
            i.parameters.shift()
            core.set_sdrp(tmp, i.parameters[0])
            break
          case 'set_reactive_separate':
            tmp = this.validate_sid(i.parameters[0], job)
            if (!tmp) {
              return
            }
            i.parameters.shift()
            core.set_reactive_separate(tmp, i.parameters[0])
            break
          case 'set_t_injection':
            tmp = this.validate_sid(i.parameters[0], job)
            if (!tmp) {
              return
            }
            i.parameters.shift()
            core.set_t_injection(tmp, i.parameters[0])
            break
          case 'prob':
            tmp = this.validate_sid(i.parameters[0], job)
            if (!tmp) {
              return
            }
            i.parameters.shift()
            await outputService.createOrUpdate(job.id, i.output, core.prob(tmp, i.parameters[0]), 3)
            break
          case 'prob_rdm':
            tmp = this.validate_sid(i.parameters[0], job)
            if (!tmp) {
              return
            }
            i.parameters.shift()
            await outputService.createOrUpdate(job.id, i.output, core.prob_rdm(tmp, i.parameters[0]), 3)
            break
          case 'perm_prob':
            tmp = this.validate_sid(i.parameters[0], job)
            if (!tmp) {
              return
            }
            i.parameters.shift()
            tmpLongVec = core.VectorLong(i.parameters[0])
            i.parameters.shift()
            tmpCharVec = core.VectorLong(i.parameters[0])
            i.parameters.shift()
            await outputService.createOrUpdate(job.id, i.output, core.perm_prob(tmp, tmpLongVec, tmpCharVec), 3)
            break
          case 'perm_prob_rdm':
            tmp = this.validate_sid(i.parameters[0], job)
            if (!tmp) {
              return
            }
            i.parameters.shift()
            tmpLongVec = core.VectorLong(i.parameters[0])
            i.parameters.shift()
            tmpCharVec = core.VectorChar(i.parameters[0])
            i.parameters.shift()
            await outputService.createOrUpdate(job.id, i.output, core.perm_prob_rdm(tmp, tmpLongVec, tmpCharVec, i.parameters[0]), 3)
            break
          case 'fact_exp':
            tmp = this.validate_sid(i.parameters[0], job)
            if (!tmp) {
              return
            }
            i.parameters.shift()
            tmpLongVec = core.VectorLong(i.parameters[0])
            i.parameters.shift()
            tmpLongVec2 = core.VectorLong(i.parameters[0])
            i.parameters.shift()
            await outputService.createOrUpdate(job.id, i.output, core.fact_exp(tmp, tmpLongVec, tmpLongVec2), 3)
            break
          case 'fact_exp_rdm':
            tmp = this.validate_sid(i.parameters[0], job)
            if (!tmp) {
              return
            }
            i.parameters.shift()
            tmpLongVec = core.VectorLong(i.parameters[0])
            i.parameters.shift()
            tmpLongVec2 = core.VectorLong(i.parameters[0])
            i.parameters.shift()
            await outputService.createOrUpdate(job.id, i.output, core.fact_exp_rdm(tmp, tmpLongVec, tmpLongVec2, i.parameters[0]), 3)
            break
          case 'fact_exp_fp':
            tmp = this.validate_sid(i.parameters[0], job)
            if (!tmp) {
              return
            }
            i.parameters.shift()
            tmpLongVec = core.VectorLong(i.parameters[0])
            i.parameters.shift()
            tmpDoubleVec = core.VectorDouble(i.parameters[0])
            i.parameters.shift()
            await outputService.createOrUpdate(job.id, i.output, core.fact_exp_fp(tmp, tmpLongVec, tmpDoubleVec), 3)
            break
          case 'fact_exp_rdm':
            tmp = this.validate_sid(i.parameters[0], job)
            if (!tmp) {
              return
            }
            i.parameters.shift()
            tmpLongVec = core.VectorLong(i.parameters[0])
            i.parameters.shift()
            tmpDoubleVec = core.VectorDouble(i.parameters[0])
            i.parameters.shift()
            await outputService.createOrUpdate(job.id, i.output, core.fact_exp_rdm(tmp, tmpLongVec, tmpDoubleVec, i.parameters[0]), 3)
            break
          case 'phase_parity':
            tmp = this.validate_sid(i.parameters[0], job)
            if (!tmp) {
              return
            }
            i.parameters.shift()
            tmpLongVec = core.VectorLong(i.parameters[0])
            i.parameters.shift()
            core.phase_parity(tmp, tmpLongVec, i.parameters[0])
            break
          case 'joint_ensemble_prob':
            tmp = this.validate_sid(i.parameters[0], job)
            if (!tmp) {
              return
            }
            i.parameters.shift()
            tmpLongVec = core.VectorLong(i.parameters[0])
            i.parameters.shift()
            tmpCharVec = core.VectorChar(i.parameters[0])
            i.parameters.shift()
            await outputService.createOrUpdate(job.id, i.output, core.joint_ensemble_prob(tmp, tmpLongVec, tmpCharVec), 3)
            break
          case 'compose':
            tmp = this.validate_sid(i.parameters[0], job)
            if (!tmp) {
              return
            }
            i.parameters.shift()
            tmp2 = this.validate_sid(i.parameters[0], job)
            if (!tmp2) {
              return
            }
            i.parameters.shift()
            tmpLongVec = core.VectorLong(i.parameters[0])
            i.parameters.shift()
            core.compose(tmp, tmp2, tmpLongVec)
            break
          case 'decompose':
            tmp = this.validate_sid(i.parameters[0], job)
            if (!tmp) {
              return
            }
            i.parameters.shift()
            tmpLongVec = core.VectorLong(i.parameters[0])
            i.parameters.shift()
            await outputService.createOrUpdate(job.id, i.output, core.decompose(tmp, tmpLongVec), 1)
            break
          case 'dispose':
            tmp = this.validate_sid(i.parameters[0], job)
            if (!tmp) {
              return
            }
            i.parameters.shift()
            tmpLongVec = core.VectorLong(i.parameters[0])
            i.parameters.shift()
            core.dispose(tmp, tmpLongVec)
            break
          default:
            // Job status 2: FAILURE
            job.jobStatusTypeId = 2
            job.statusMessage = 'One or more of your job program operation line names do not match a defined operation name.'
            await job.save()
            return
        }
      }

      // Job status 1: SUCCESS
      job.jobStatusTypeId = 1
      job.statusMessage = 'Job completed fully and normally.'
      await job.save()
    }).error(async (e) => {
      // Job status 2: FAILURE
      job.jobStatusTypeId = 2
      job.statusMessage = e.toString()
      await job.save()
    })

    return { success: true, body: await this.sanitize(job) }
  }

  async delete (jobId) {
    const job = await this.getByPk(jobId)
    if (!job) {
      return { success: false, error: 'Job ID not found.' }
    }

    await job.destroy()

    return { success: true, body: this.sanitize(job) }
  }
}

module.exports = JobService
