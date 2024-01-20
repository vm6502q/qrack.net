// jobService.js

const path = require('path')

// Database Model
const db = require('../model/index')

// Data Access Layer
const ModelService = require('./modelService')

// Services
const OutputService = require('./outputService')

// Qrack
const Qrack = require('../Qrack')

const qrack = Qrack({
  locateFile: () => {
    return path.resolve(__dirname, '../Qrack.wasm')
  }
}).catch((e) => {
  console.log('Could not load Qrack: ' + e.toString())
})

const outputService = new OutputService()
const Job = db.job

class JobService extends ModelService {
  constructor () {
    super(Job)
  }

  async get (jobId, userId) {
    const job = await this.getByPk(jobId)
    if (!job) {
      return { success: false, error: 'Job ID not found.' }
    }
    if (job.userId !== userId) {
      return { success: false, error: 'You are not authorized for this Job ID.' }
    }

    return { success: true, body: job }
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

  async single_quid_op (job, fn, i) {
    const tmp = this.validate_sid(i.parameters[0], job)
    if (!tmp) {
      return true
    }
    i.parameters.shift()
    fn(tmp, ...i.parameters)

    return false
  }

  async single_quid_output_op (job, fn, i, oType) {
    const tmp = this.validate_sid(i.parameters[0], job)
    if (!tmp) {
      return true
    }
    i.parameters.shift()
    await outputService.createOrUpdate(job.id, i.output, fn(tmp, ...i.parameters), oType)

    return false
  }

  async single_quid_mc_op (job, fn, i, core) {
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

  async single_quid_mc_output_op (job, fn, i, oType, core) {
    const tmp = this.validate_sid(i.parameters[0], job)
    if (!tmp) {
      return true
    }
    i.parameters.shift()
    const tmpLongVec = core.VectorLong(i.parameters[0])
    i.parameters.shift()
    await outputService.createOrUpdate(job.id, i.output, fn(tmp, tmpLongVec, ...i.parameters), oType)

    return false
  }

  async single_quid_mc_pauli_output_op (job, fn, i, oType, core) {
    const tmp = this.validate_sid(i.parameters[0], job)
    if (!tmp) {
      return true
    }
    i.parameters.shift()
    const tmpLongVec = core.VectorLong(i.parameters[0])
    i.parameters.shift()
    const tmpCharVec = core.VectorChar(i.parameters[0])
    i.parameters.shift()
    await outputService.createOrUpdate(job.id, i.output, fn(tmp, tmpLongVec, tmpCharVec, ...i.parameters), oType)

    return false
  }

  async single_quid_mc_mtrx_op (job, fn, i, core) {
    const tmp = this.validate_sid(i.parameters[0], job)
    if (!tmp) {
      return true
    }
    i.parameters.shift()
    const tmpLongVec = core.VectorLong(i.parameters[0])
    i.parameters.shift()
    const tmpDoubleVec = core.VectorDouble(i.parameters[0])
    i.parameters.shift()
    fn(tmp, tmpLongVec, tmpDoubleVec, ...i.parameters)

    return false
  }

  async single_quid_mc2_output_op (job, fn, i, oType, core) {
    const tmp = this.validate_sid(i.parameters[0], job)
    if (!tmp) {
      return true
    }
    i.parameters.shift()
    const tmpLongVec = core.VectorLong(i.parameters[0])
    i.parameters.shift()
    const tmpLongVec2 = core.VectorLong(i.parameters[0])
    i.parameters.shift()
    await outputService.createOrUpdate(job.id, i.output, fn(tmp, tmpLongVec, tmpLongVec2, ...i.parameters), oType)

    return false
  }

  async single_quid_mc_double_output_op (job, fn, i, oType, core) {
    const tmp = this.validate_sid(i.parameters[0], job)
    if (!tmp) {
      return true
    }
    i.parameters.shift()
    const tmpLongVec = core.VectorLong(i.parameters[0])
    i.parameters.shift()
    const tmpDoubleVec = core.VectorDouble(i.parameters[0])
    i.parameters.shift()
    await outputService.createOrUpdate(job.id, i.output, fn(tmp, tmpLongVec, tmpDoubleVec, ...i.parameters), oType)

    return false
  }

  async runQrackProgram (core, p, job) {
    let tmp, tmp2, tmpLongVec, tmpCharVec, tmpDoubleVec
    for (const i in p) {
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
          if (this.single_quid_output_op(job, core.init_clone, i, 1)) {
            return
          }
          break
        case 'destroy':
          if (this.single_quid_op(job, core.destroy, i)) {
            return
          }
          break
        case 'seed':
          if (this.single_quid_op(job, core.seed, i)) {
            return
          }
          break
        case 'try_separate_1qb':
          if (this.single_quid_output_op(job, core.try_separate_1qb, i, 2)) {
            return
          }
          break
        case 'try_separate_2qb':
          if (this.single_quid_output_op(job, core.try_separate_2qb, i, 2)) {
            return
          }
          break
        case 'try_separate_tol':
          if (this.single_quid_mc_output_op(job, core.try_separate_tol, i, 2)) {
            return
          }
          break
        case 'get_unitary_fidelity':
          if (this.single_quid_output_op(job, core.get_unitary_fidelity, i, 2)) {
            return
          }
          break
        case 'reset_unitary_fidelity':
          if (this.single_quid_op(job, core.reset_unitary_fidelity, i)) {
            return
          }
          break
        case 'set_sdrp':
          if (this.single_quid_op(job, core.set_sdrp, i)) {
            return
          }
          break
        case 'set_reactive_separate':
          if (this.single_quid_op(job, core.set_reactive_separate, i)) {
            return
          }
          break
        case 'set_t_injection':
          if (this.single_quid_op(job, core.set_t_injection, i)) {
            return
          }
          break
        case 'prob':
          if (this.single_quid_output_op(job, core.prob, i, 3)) {
            return
          }
          break
        case 'prob_rdm':
          if (this.single_quid_output_op(job, core.prob_rdm, i, 3)) {
            return
          }
          break
        case 'perm_prob':
          if (this.single_quid_mc2_output_op(job, core.perm_prob, i, 3)) {
            return
          }
          break
        case 'perm_prob_rdm':
          if (this.single_quid_mc_pauli_output_op(job, core.perm_prob_rdm, i, 3)) {
            return
          }
          break
        case 'fact_exp':
          if (this.single_quid_mc2_output_op(job, core.fact_exp, i, 3)) {
            return
          }
          break
        case 'fact_exp_rdm':
          if (this.single_quid_mc2_output_op(job, core.fact_exp_rdm, i, 3)) {
            return
          }
          break
        case 'fact_exp_fp':
          if (this.single_quid_mc_double_output_op(job, core.fact_exp_fp, i, 3)) {
            return
          }
          break
        case 'fact_exp_fp_rdm':
          if (this.single_quid_mc_double_output_op(job, core.fact_exp_fp_rdm, i, 3)) {
            return
          }
          break
        case 'phase_parity':
          if (this.single_quid_mc_op(job, core.phase_parity, i)) {
            return
          }
          break
        case 'joint_ensemble_prob':
          if (this.single_quid_mc_pauli_output_op(job, core.joint_ensemble_prob, i, 3)) {
            return
          }
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
          if (this.single_quid_mc_output_op(job, core.decompose, i, 1)) {
            return
          }
          break
        case 'dispose':
          if (this.single_quid_mc_op(job, core.dispose, i)) {
            return
          }
          break
        case 'reset_all':
          if (this.single_quid_op(job, core.reset_all, i)) {
            return
          }
          break
        case 'measure':
          if (this.single_quid_output_op(job, core.measure, i, 2)) {
            return
          }
          break
        case 'force_measure':
          if (this.single_quid_output_op(job, core.force_measure, i, 2)) {
            return
          }
          break
        case 'measure_basis':
          if (this.single_quid_mc_pauli_output_op(job, core.measure_basis, i, 3)) {
            return
          }
          break
        case 'measure_all':
          if (this.single_quid_output_op(job, core.measure_all, i, 4)) {
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
          if (this.single_quid_op(job, core.x, i)) {
            return
          }
          break
        case 'y':
          if (this.single_quid_op(job, core.y, i)) {
            return
          }
          break
        case 'z':
          if (this.single_quid_op(job, core.z, i)) {
            return
          }
          break
        case 'h':
          if (this.single_quid_op(job, core.h, i)) {
            return
          }
          break
        case 's':
          if (this.single_quid_op(job, core.s, i)) {
            return
          }
          break
        case 't':
          if (this.single_quid_op(job, core.t, i)) {
            return
          }
          break
        case 'adjs':
          if (this.single_quid_op(job, core.adjs, i)) {
            return
          }
          break
        case 'adjt':
          if (this.single_quid_op(job, core.adjt, i)) {
            return
          }
          break
        case 'u':
          if (this.single_quid_op(job, core.u, i)) {
            return
          }
          break
        case 'mtrx':
          tmp = this.validate_sid(i.parameters[0], job)
          if (!tmp) {
            return
          }
          i.parameters.shift()
          tmpDoubleVec = core.VectorDouble(i.parameters[0])
          i.parameters.shift()
          core.mtrx(tmp, tmpDoubleVec, ...i.parameters)
          break
        case 'mcx':
          if (this.single_quid_mc_op(job, core.mcx, i)) {
            return
          }
          break
        case 'mcy':
          if (this.single_quid_mc_op(job, core.mcy, i)) {
            return
          }
          break
        case 'mcz':
          if (this.single_quid_mc_op(job, core.mcz, i)) {
            return
          }
          break
        case 'mch':
          if (this.single_quid_mc_op(job, core.mch, i)) {
            return
          }
          break
        case 'mcs':
          if (this.single_quid_mc_op(job, core.mcs, i)) {
            return
          }
          break
        case 'mct':
          if (this.single_quid_mc_op(job, core.mct, i)) {
            return
          }
          break
        case 'mcadjs':
          if (this.single_quid_mc_op(job, core.mcadjs, i)) {
            return
          }
          break
        case 'mcadjt':
          if (this.single_quid_mc_op(job, core.mcadjt, i)) {
            return
          }
          break
        case 'mcu':
          if (this.single_quid_mc_op(job, core.mcu, i)) {
            return
          }
          break
        case 'mcmtrx':
          if (this.single_quid_mc_mtrx_op(job, core.mcmtrx, i)) {
            return
          }
          break
        case 'macx':
          if (this.single_quid_mc_op(job, core.macx, i)) {
            return
          }
          break
        case 'macy':
          if (this.single_quid_mc_op(job, core.macy, i)) {
            return
          }
          break
        case 'macz':
          if (this.single_quid_mc_op(job, core.macz, i)) {
            return
          }
          break
        case 'mach':
          if (this.single_quid_mc_op(job, core.mach, i)) {
            return
          }
          break
        case 'macs':
          if (this.single_quid_mc_op(job, core.macs, i)) {
            return
          }
          break
        case 'mact':
          if (this.single_quid_mc_op(job, core.mact, i)) {
            return
          }
          break
        case 'macadjs':
          if (this.single_quid_mc_op(job, core.macadjs, i)) {
            return
          }
          break
        case 'macadjt':
          if (this.single_quid_mc_op(job, core.macadjt, i)) {
            return
          }
          break
        case 'macu':
          if (this.single_quid_mc_op(job, core.macu, i)) {
            return
          }
          break
        case 'macmtrx':
          if (this.single_quid_mc_mtrx_op(job, core.macmtrx, i)) {
            return
          }
          break
        case 'ucmtrx':
          if (this.single_quid_mc_mtrx_op(job, core.ucmtrx, i)) {
            return
          }
          break
        case 'multiplex_1qb_mtrx':
          if (this.single_quid_mc_mtrx_op(job, core.multiplex_1qb_mtrx, i)) {
            return
          }
          break
        case 'mx':
          if (this.single_quid_mc_op(job, core.mx, i)) {
            return
          }
          break
        case 'my':
          if (this.single_quid_mc_op(job, core.my, i)) {
            return
          }
          break
        case 'mz':
          if (this.single_quid_mc_op(job, core.mz, i)) {
            return
          }
          break
        case 'r':
          if (this.single_quid_op(job, core.r, i)) {
            return
          }
          break
        case 'mcr':
          if (this.single_quid_mc_op(job, core.mcr, i)) {
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
  }

  validateCreateRequest (reqBody) {
    console.log(reqBody)
    if (reqBody.program === undefined) {
      return { success: false, error: 'Job creation request must contain the "program" parameter.' }
    }
    if (!Array.isArray(reqBody.program)) {
      return { success: false, error: 'Job creation request "program" parameter must be an array.' }
    }
    return { success: true }
  }

  async new (reqBody, userId) {
    const validationResult = this.validateCreateRequest(reqBody)
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

    qrack.then(async (core) => { await this.runQrackProgram(core, reqBody.program, job) })
      .catch(async (e) => {
        // Job status 2: FAILURE
        job.jobStatusTypeId = 2
        job.statusMessage = e.toString()
        await job.save()
      })

    return { success: true, body: job }
  }

  async getStatusAndOutput (jobId) {
    const job = await this.getByPk(jobId)
    if (!job) {
      return { success: false, error: 'Job ID not found.' }
    }

    const outputArray = await outputService.getByJobId(jobId)
    const output = {}
    for (const p in outputArray) {
      output[p.name] = p.value
    }

    return { success: true, body: { status: job.status, output } }
  }

  async delete (jobId) {
    const job = await this.getByPk(jobId)
    if (!job) {
      return { success: false, error: 'Job ID not found.' }
    }

    await job.destroy()

    return { success: true, body: job }
  }
}

module.exports = JobService
