// jobService.js

const fs = require('node:fs')
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
    return path.resolve(__dirname, '../Qrack.wasm');
  }
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
    if (job.userId != userId) {
      return { success: false, error: 'You are not authorized for this Job ID.' }
    }

    return { success: true, body: job }
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

  async single_quid_op (job, fn, i) {
    const tmp = this.validate_sid(i.parameters[0], job)
    if (!tmp) {
      return true
    }
    i.parameters.shift()
    fn(tmp, ...i.parameters)

    return false
  }

  async single_quid_output_op (job, fn, i, o_type) {
    const tmp = this.validate_sid(i.parameters[0], job)
    if (!tmp) {
      return true
    }
    i.parameters.shift()
    await outputService.createOrUpdate(job.id, i.output, fn(tmp, ...i.parameters), o_type)

    return false
  }

  async single_quid_mc_op (job, fn, i) {
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

  async single_quid_mc_output_op (job, fn, i, o_type) {
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

  async single_quid_mc_pauli_output_op (job, fn, i, o_type) {
    const tmp = this.validate_sid(i.parameters[0], job)
    if (!tmp) {
      return true
    }
    i.parameters.shift()
    const tmpLongVec = core.VectorLong(i.parameters[0])
    i.parameters.shift()
    const tmpCharVec = core.VectorChar(i.parameters[0])
    i.parameters.shift()
    await outputService.createOrUpdate(job.id, i.output, fn(tmp, tmpLongVec, tmpCharVec, ...i.parameters), o_type)

    return false
  }

  async single_quid_mc_mtrx_op (job, fn, i) {
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

  async single_quid_mc2_output_op (job, fn, i, o_type) {
    const tmp = this.validate_sid(i.parameters[0], job)
    if (!tmp) {
      return true
    }
    i.parameters.shift()
    const tmpLongVec = core.VectorLong(i.parameters[0])
    i.parameters.shift()
    const tmpLongVec2 = core.VectorLong(i.parameters[0])
    i.parameters.shift()
    await outputService.createOrUpdate(job.id, i.output, fn(tmp, tmpLongVec, tmpLongVec2, ...i.parameters), o_type)

    return false
  }

  async single_quid_mc_double_output_op (job, fn, i, o_type) {
    const tmp = this.validate_sid(i.parameters[0], job)
    if (!tmp) {
      return true
    }
    i.parameters.shift()
    const tmpLongVec = core.VectorLong(i.parameters[0])
    i.parameters.shift()
    const tmpDoubleVec = core.VectorDouble(i.parameters[0])
    i.parameters.shift()
    await outputService.createOrUpdate(job.id, i.output, fn(tmp, tmpLongVec, tmpDoubleVec, ...i.parameters), o_type)

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

    const p = reqBody.program
    qrack.then(async (core) => {
      let tmp, tmp2, tmpLongVec, tmpCharVec, mtrx
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
            if (single_quid_mc2_output_op(job, core.perm_prob, i, 3)) {
              return
            }
            break
          case 'perm_prob_rdm':
            if (single_quid_mc_pauli_output_op(job, core.perm_prob_rdm, i, 3)) {
              return
            }
            break
          case 'fact_exp':
            if (single_quid_mc2_output_op(job, core.fact_exp, i, 3)) {
              return
            }
            break
          case 'fact_exp_rdm':
            if (single_quid_mc2_output_op(job, core.fact_exp_rdm, i, 3)) {
              return
            }
            break
          case 'fact_exp_fp':
            if (single_quid_mc_double_output_op(job, core.fact_exp_fp, i, 3)) {
              return
            }
            break
          case 'fact_exp_rdm':
            if (single_quid_mc_double_output_op(job, core.fact_exp_rdm, i, 3)) {
              return
            }
            break
          case 'phase_parity':
            if (single_quid_mc_op(job, core.phase_parity, i)) {
              return
            }
            break
          case 'joint_ensemble_prob':
            if (single_quid_mc_pauli_output_op(job, core.joint_ensemble_prob, i, 3)) {
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
            if (single_quid_mc_pauli_output_op(job, core.measure_basis, i, 3)) {
              return
            }
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
            break
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
            if (single_quid_mc_op(job, core.mcx, i)) {
              return
            }
            break
          case 'mcy':
            if (single_quid_mc_op(job, core.mcy, i)) {
              return
            }
            break
          case 'mcz':
            if (single_quid_mc_op(job, core.mcz, i)) {
              return
            }
            break
          case 'mch':
            if (single_quid_mc_op(job, core.mch, i)) {
              return
            }
            break
          case 'mcs':
            if (single_quid_mc_op(job, core.mcs, i)) {
              return
            }
            break
          case 'mct':
            if (single_quid_mc_op(job, core.mct, i)) {
              return
            }
            break
          case 'mcadjs':
            if (single_quid_mc_op(job, core.mcadjs, i)) {
              return
            }
            break
          case 'mcadjt':
            if (single_quid_mc_op(job, core.mcadjt, i)) {
              return
            }
            break
          case 'mcu':
            if (single_quid_mc_op(job, core.mcu, i)) {
              return
            }
            break
          case 'mcmtrx':
            if (single_quid_mc_mtrx_op(job, core.mcmtrx, i)) {
              return
            }
            break
          case 'macx':
            if (single_quid_mc_op(job, core.macx, i)) {
              return
            }
            break
          case 'macy':
            if (single_quid_mc_op(job, core.macy, i)) {
              return
            }
            break
          case 'macz':
            if (single_quid_mc_op(job, core.macz, i)) {
              return
            }
            break
          case 'mach':
            if (single_quid_mc_op(job, core.mach, i)) {
              return
            }
            break
          case 'macs':
            if (single_quid_mc_op(job, core.macs, i)) {
              return
            }
            break
          case 'mact':
            if (single_quid_mc_op(job, core.mact, i)) {
              return
            }
            break
          case 'macadjs':
            if (single_quid_mc_op(job, core.macadjs, i)) {
              return
            }
            break
          case 'macadjt':
            if (single_quid_mc_op(job, core.macadjt, i)) {
              return
            }
            break
          case 'macu':
            if (single_quid_mc_op(job, core.macu, i)) {
              return
            }
            break
          case 'macmtrx':
            if (single_quid_mc_mtrx_op(job, core.macmtrx, i)) {
              return
            }
            break
          case 'ucmtrx':
            if (single_quid_mc_mtrx_op(job, core.ucmtrx, i)) {
              return
            }
            break
          case 'multiplex_1qb_mtrx':
            if (single_quid_mc_mtrx_op(job, core.multiplex_1qb_mtrx, i)) {
              return
            }
            break
          case 'mx':
            if (single_quid_mc_op(job, core.mx, i)) {
              return
            }
            break
          case 'my':
            if (single_quid_mc_op(job, core.my, i)) {
              return
            }
            break
          case 'mz':
            if (single_quid_mc_op(job, core.mz, i)) {
              return
            }
            break
          case 'r':
            if (single_quid_op(job, core.r, i)) {
              return
            }
            break
          case 'mcr':
            if (single_quid_mc_op(job, core.mcr, i)) {
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

    return { success: true, body: job }
  }

  async getStatusAndOutput (jobId) {
    const job = await this.getByPk(jobId)
    if (!job) {
      return { success: false, error: 'Job ID not found.' }
    }

    const outputArray = await outputService.getByJobId(jobId)
    const output = {}
    for (p in outputArray) {
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
