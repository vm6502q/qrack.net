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

  async single_quid_op(job, fn, i) {
    const tmp = this.validate_sid(i.parameters[0], job)
    if (!tmp) {
      return true
    }
    i.parameters.shift()
    fn(tmp, ...i.parameters)

    return false
  }

  async single_quid_output_op(job, fn, i, o_type) {
    const tmp = this.validate_sid(i.parameters[0], job)
    if (!tmp) {
      return true
    }
    i.parameters.shift()
    await outputService.createOrUpdate(job.id, i.output, fn(tmp, ...i.parameters), o_type)

    return false
  }

  async single_quid_mc_op(job, fn, i) {
    const tmp = this.validate_sid(i.parameters[0], job)
    if (!tmp) {
      return true
    }
    i.parameters.shift()
    const tmpLongVec = core.VectorLong(i.parameters[0])
    i.parameters.shift()
    fn(tmp, tmpLongVec, ...i.parameters)

    return false
  }

  async single_quid_mc_output_op(job, fn, i, o_type) {
    const tmp = this.validate_sid(i.parameters[0], job)
    if (!tmp) {
      return true
    }
    i.parameters.shift()
    const tmpLongVec = core.VectorLong(i.parameters[0])
    i.parameters.shift()
    await outputService.createOrUpdate(job.id, i.output, fn(tmp, tmpLongVec, ...i.parameters), o_type)

    return false
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
            if (single_quid_output_op(job, core.init_clone, i, 1)) {
              return
            }
            break
          case 'destroy':
            if (single_quid_op(job, core.destroy, i)) {
              return
            }
            break
          case 'seed':
            if (single_quid_op(job, core.seed, i)) {
              return
            }
            break
          case 'try_separate_1qb':
            if (single_quid_output_op(job, core.try_separate_1qb, i, 2)) {
              return
            }
            break
          case 'try_separate_2qb':
            if (single_quid_output_op(job, core.try_separate_2qb, i, 2)) {
              return
            }
            break
          case 'try_separate_tol':
            if (single_quid_mc_output_op(job, core.try_separate_tol, i, 2)) {
              return
            }
            break
          case 'get_unitary_fidelity':
            if (single_quid_output_op(job, core.get_unitary_fidelity, i, 2)) {
              return
            }
            break
          case 'reset_unitary_fidelity':
            if (single_quid_op(job, core.reset_unitary_fidelity, i)) {
              return
            }
            break
          case 'set_sdrp':
            if (single_quid_op(job, core.set_sdrp, i)) {
              return
            }
            break
          case 'set_reactive_separate':
            if (single_quid_op(job, core.set_reactive_separate, i)) {
              return
            }
            break
          case 'set_t_injection':
            if (single_quid_op(job, core.set_t_injection, i)) {
              return
            }
            break
          case 'prob':
            if (single_quid_output_op(job, core.prob, i, 3)) {
              return
            }
            break
          case 'prob_rdm':
            if (single_quid_output_op(job, core.prob_rdm, i, 3)) {
              return
            }
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
            if (single_quid_mc_op(job, core.phase_parity, i)) {
              return
            }
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
            if (single_quid_mc_output_op(job, core.decompose, i, 1)) {
              return
            }
            break
          case 'dispose':
            if (single_quid_mc_op(job, core.dispose, i)) {
              return
            }
            break
          case 'reset_all':
            if (single_quid_op(job, core.reset_all, i)) {
              return
            }
            break
          case 'measure':
            if (single_quid_output_op(job, core.measure, i, 2)) {
              return
            }
            break
          case 'force_measure':
            if (single_quid_output_op(job, core.force_measure, i, 2)) {
              return
            }
            break
          case 'measure_basis':
            tmp = this.validate_sid(i.parameters[0], job)
            if (!tmp) {
              return
            }
            i.parameters.shift()
            tmpLongVec = core.VectorLong(i.parameters[0])
            i.parameters.shift()
            tmpCharVec = core.VectorChar(i.parameters[0])
            i.parameters.shift()
            await outputService.createOrUpdate(job.id, i.output, core.measure_basis(tmp, tmpLongVec, tmpCharVec), 1)
            break
          case 'measure_all':
            if (single_quid_output_op(job, core.measure_all, i, 4)) {
              return
            }
            break
          case 'measure_shots':
            tmp = this.validate_sid(i.parameters[0], job)
            if (!tmp) {
              return
            }
            i.parameters.shift()
            tmpLongVec = core.VectorLong(i.parameters[0])
            i.parameters.shift()
            tmpCharVec = core.VectorChar(i.parameters[0])
            i.parameters.shift()
            tmpLongVec = core.measure_shots(tmp, tmpLongVec, tmpCharVec)
            // See https://github.com/emscripten-core/emscripten/issues/11070
            await outputService.createOrUpdate(job.id, i.output, new Array(tmpLongVec.size()).fill(0).map((_, id) => tmpLongVec.get(id)), 5)
            break
          case 'x':
            if (single_quid_op(job, core.x, i)) {
              return
            }
            break;
          case 'y':
            if (single_quid_op(job, core.y, i)) {
              return
            }
            break
          case 'z':
            if (single_quid_op(job, core.z, i)) {
              return
            }
            break
          case 'h':
            if (single_quid_op(job, core.h, i)) {
              return
            }
            break
          case 's':
            if (single_quid_op(job, core.s, i)) {
              return
            }
            break
          case 't':
            if (single_quid_op(job, core.t, i)) {
              return
            }
            break
          case 'adjs':
            if (single_quid_op(job, core.adjs, i)) {
              return
            }
            break
          case 'adjt':
            if (single_quid_op(job, core.adjt, i)) {
              return
            }
            break
          case 'u':
            if (single_quid_op(job, core.u, i)) {
              return
            }
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
