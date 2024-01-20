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

  async validate_sid (name, job) {
    const v = await outputService.getByJobIdAndName(job.id, name)
    if (!v) {
      throw new Error(
        'All simulator IDs and quantum neuron IDs should be specified as names in the output space of the job. ' +
        '(Methods that produce output, at all, always save it to the "output space" of the job, to the ' +
        'variable named by the "output" parameter of the job program line that produces output.)'
      )
    }
    switch (v.outputTypeId) {
      case 1:
        return parseInt(v.value)
      default:
        throw new Error("Quid instruction parameter does not have quid outputTypeId.")
    }
  }

  async single_quid_op (job, fn, i) {
    const tmp = this.validate_sid(i.parameters[0], job)
    i.parameters.shift()
    fn(tmp, ...i.parameters)
  }

  async single_quid_output_op (job, fn, i, oType) {
    const tmp = this.validate_sid(i.parameters[0], job)
    i.parameters.shift()
    await outputService.createOrUpdate(job.id, i.output, fn(tmp, ...i.parameters), oType)
  }

  async single_quid_mc_op (job, fn, i, core) {
    const tmp = this.validate_sid(i.parameters[0], job)
    i.parameters.shift()
    const tmpLongVec = core.VectorLong(i.parameters[0])
    i.parameters.shift()
    fn(tmp, tmpLongVec, ...i.parameters)

    return false
  }

  async single_quid_mc_output_op (job, fn, i, oType, core) {
    const tmp = this.validate_sid(i.parameters[0], job)
    i.parameters.shift()
    const tmpLongVec = core.VectorLong(i.parameters[0])
    i.parameters.shift()
    await outputService.createOrUpdate(job.id, i.output, fn(tmp, tmpLongVec, ...i.parameters), oType)

    return false
  }

  async single_quid_mc_pauli_output_op (job, fn, i, oType, core) {
    const tmp = this.validate_sid(i.parameters[0], job)
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
          this.single_quid_op(job, core.destroy, i)
          break
        case 'seed':
          this.single_quid_op(job, core.seed, i)
          break
        case 'try_separate_1qb':
          this.single_quid_output_op(job, core.try_separate_1qb, i, 2)
          break
        case 'try_separate_2qb':
          this.single_quid_output_op(job, core.try_separate_2qb, i, 2)
          break
        case 'try_separate_tol':
          this.single_quid_mc_output_op(job, core.try_separate_tol, i, 2)
          break
        case 'get_unitary_fidelity':
          this.single_quid_output_op(job, core.get_unitary_fidelity, i, 2)
          break
        case 'reset_unitary_fidelity':
          this.single_quid_op(job, core.reset_unitary_fidelity, i)
          break
        case 'set_sdrp':
          this.single_quid_op(job, core.set_sdrp, i)
          break
        case 'set_reactive_separate':
          this.single_quid_op(job, core.set_reactive_separate, i)
          break
        case 'set_t_injection':
          this.single_quid_op(job, core.set_t_injection, i)
          break
        case 'prob':
          this.single_quid_output_op(job, core.prob, i, 3)
          break
        case 'prob_rdm':
          this.single_quid_output_op(job, core.prob_rdm, i, 3)
          break
        case 'perm_prob':
          this.single_quid_mc2_output_op(job, core.perm_prob, i, 3)
          break
        case 'perm_prob_rdm':
          this.single_quid_mc_pauli_output_op(job, core.perm_prob_rdm, i, 3)
          break
        case 'fact_exp':
          this.single_quid_mc2_output_op(job, core.fact_exp, i, 3)
          break
        case 'fact_exp_rdm':
          this.single_quid_mc2_output_op(job, core.fact_exp_rdm, i, 3)
          break
        case 'fact_exp_fp':
          this.single_quid_mc_double_output_op(job, core.fact_exp_fp, i, 3)
          break
        case 'fact_exp_fp_rdm':
          this.single_quid_mc_double_output_op(job, core.fact_exp_fp_rdm, i, 3)
          break
        case 'phase_parity':
          this.single_quid_mc_op(job, core.phase_parity, i)
          break
        case 'joint_ensemble_prob':
          this.single_quid_mc_pauli_output_op(job, core.joint_ensemble_prob, i, 3)
          break
        case 'compose':
          tmp = this.validate_sid(i.parameters[0], job)
          i.parameters.shift()
          tmp2 = this.validate_sid(i.parameters[0], job)
          i.parameters.shift()
          tmpLongVec = core.VectorLong(i.parameters[0])
          i.parameters.shift()
          core.compose(tmp, tmp2, tmpLongVec)
          break
        case 'decompose':
          this.single_quid_mc_output_op(job, core.decompose, i, 1)
          break
        case 'dispose':
          this.single_quid_mc_op(job, core.dispose, i)
          break
        case 'reset_all':
          this.single_quid_op(job, core.reset_all, i)
          break
        case 'measure':
          this.single_quid_output_op(job, core.measure, i, 2)
          break
        case 'force_measure':
          this.single_quid_output_op(job, core.force_measure, i, 2)
          break
        case 'measure_basis':
          this.single_quid_mc_pauli_output_op(job, core.measure_basis, i, 3)
          break
        case 'measure_all':
          this.single_quid_output_op(job, core.measure_all, i, 4)
          break
        case 'measure_shots':
          tmp = this.validate_sid(i.parameters[0], job)
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
          this.single_quid_op(job, core.x, i)
          break
        case 'y':
          this.single_quid_op(job, core.y, i)
          break
        case 'z':
          this.single_quid_op(job, core.z, i)
          break
        case 'h':
          this.single_quid_op(job, core.h, i)
          break
        case 's':
          this.single_quid_op(job, core.s, i)
          break
        case 't':
          this.single_quid_op(job, core.t, i)
          break
        case 'adjs':
          this.single_quid_op(job, core.adjs, i)
          break
        case 'adjt':
          this.single_quid_op(job, core.adjt, i)
          break
        case 'u':
          this.single_quid_op(job, core.u, i)
          break
        case 'mtrx':
          tmp = this.validate_sid(i.parameters[0], job)
          i.parameters.shift()
          tmpDoubleVec = core.VectorDouble(i.parameters[0])
          i.parameters.shift()
          core.mtrx(tmp, tmpDoubleVec, ...i.parameters)
          break
        case 'mcx':
          this.single_quid_mc_op(job, core.mcx, i)
          break
        case 'mcy':
          this.single_quid_mc_op(job, core.mcy, i)
          break
        case 'mcz':
          this.single_quid_mc_op(job, core.mcz, i)
          break
        case 'mch':
          this.single_quid_mc_op(job, core.mch, i)
          break
        case 'mcs':
          this.single_quid_mc_op(job, core.mcs, i)
          break
        case 'mct':
          this.single_quid_mc_op(job, core.mct, i)
          break
        case 'mcadjs':
          this.single_quid_mc_op(job, core.mcadjs, i)
          break
        case 'mcadjt':
          this.single_quid_mc_op(job, core.mcadjt, i)
          break
        case 'mcu':
          this.single_quid_mc_op(job, core.mcu, i)
          break
        case 'mcmtrx':
          this.single_quid_mc_mtrx_op(job, core.mcmtrx, i)
          break
        case 'macx':
          this.single_quid_mc_op(job, core.macx, i)
          break
        case 'macy':
          this.single_quid_mc_op(job, core.macy, i)
          break
        case 'macz':
          this.single_quid_mc_op(job, core.macz, i)
          break
        case 'mach':
          this.single_quid_mc_op(job, core.mach, i)
          break
        case 'macs':
          this.single_quid_mc_op(job, core.macs, i)
          break
        case 'mact':
          this.single_quid_mc_op(job, core.mact, i)
          break
        case 'macadjs':
          this.single_quid_mc_op(job, core.macadjs, i)
          break
        case 'macadjt':
          this.single_quid_mc_op(job, core.macadjt, i)
          break
        case 'macu':
          this.single_quid_mc_op(job, core.macu, i)
          break
        case 'macmtrx':
          this.single_quid_mc_mtrx_op(job, core.macmtrx, i)
          break
        case 'ucmtrx':
          this.single_quid_mc_mtrx_op(job, core.ucmtrx, i)
          break
        case 'multiplex_1qb_mtrx':
          this.single_quid_mc_mtrx_op(job, core.multiplex_1qb_mtrx, i)
          break
        case 'mx':
          this.single_quid_mc_op(job, core.mx, i)
          break
        case 'my':
          this.single_quid_mc_op(job, core.my, i)
          break
        case 'mz':
          this.single_quid_mc_op(job, core.mz, i)
          break
        case 'r':
          this.single_quid_op(job, core.r, i)
          break
        case 'mcr':
          this.single_quid_mc_op(job, core.mcr, i)
          break
        default:
          throw new Error('One or more of your job program operation line names do not match a defined operation name.')
      }
    }

    // Job status 1: SUCCESS
    job.jobStatusTypeId = 1
    job.statusMessage = 'Job completed fully and normally.'
    await job.save()
  }

  validateCreateRequest (reqBody) {
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
    job.statusMessage = "RUNNING"

    const result = await this.create(job)
    if (!result.success) {
      return result
    }

    job = result.body
    await job.save()

    qrack.then(async (core) => { await this.runQrackProgram(core, reqBody.program, job) })
      .catch(async (e) => {
        // Job status 2: FAILURE
        console.log(job)
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
      output[p.name] = p.valuerun
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
