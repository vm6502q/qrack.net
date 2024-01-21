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
    const tmpIntVec = new core.VectorInt()
    for (let j = 0; j < i.parameters[0].length; ++j) {
      tmpIntVec.push_back(i.parameters[0][j])
    }
    i.parameters.shift()
    fn(tmp, tmpIntVec, ...i.parameters)
    tmpIntVec.delete()

    return false
  }

  async single_quid_mc_output_op (job, fn, i, oType, core) {
    const tmp = this.validate_sid(i.parameters[0], job)
    i.parameters.shift()
    const tmpIntVec = new core.VectorInt()
    for (let j = 0; j < i.parameters[0].length; ++j) {
      tmpIntVec.push_back(i.parameters[0][j])
    }
    i.parameters.shift()
    await outputService.createOrUpdate(job.id, i.output, fn(tmp, tmpIntVec, ...i.parameters), oType)
    tmpIntVec.delete()

    return false
  }

  async single_quid_mc_pauli_output_op (job, fn, i, oType, core) {
    const tmp = this.validate_sid(i.parameters[0], job)
    i.parameters.shift()
    const tmpIntVec = new core.VectorInt()
    for (let j = 0; j < i.parameters[0].length; ++j) {
      tmpIntVec.push_back(i.parameters[0][j])
    }
    i.parameters.shift()
    const tmpCharVec = new core.VectorChar()
    for (let j = 0; j < i.parameters[0].length; ++j) {
      tmpCharVec.push_back(i.parameters[0][j])
    }
    i.parameters.shift()
    await outputService.createOrUpdate(job.id, i.output, fn(tmp, tmpIntVec, tmpCharVec, ...i.parameters), oType)
    tmpIntVec.delete()
    tmpCharVec.delete()

    return false
  }

  async single_quid_mc_mtrx_op (job, fn, i, core) {
    const tmp = this.validate_sid(i.parameters[0], job)
    i.parameters.shift()
    const tmpIntVec = new core.VectorInt()
    for (let j = 0; j < i.parameters[0].length; ++j) {
      tmpIntVec.push_back(i.parameters[0][j])
    }
    i.parameters.shift()
    const tmpDoubleVec = new core.VectorInt()
    for (let j = 0; j < i.parameters[0].length; ++j) {
      tmpDoubleVec.push_back(i.parameters[0][j])
    }
    i.parameters.shift()
    fn(tmp, tmpIntVec, tmpDoubleVec, ...i.parameters)
    tmpIntVec.delete()
    tmpDoubleVec.delete()

    return false
  }

  async single_quid_mc2_output_op (job, fn, i, oType, core) {
    const tmp = this.validate_sid(i.parameters[0], job)
    i.parameters.shift()
    const tmpIntVec = new core.VectorInt()
    for (let j = 0; j < i.parameters[0].length; ++j) {
      tmpIntVec.push_back(i.parameters[0][j])
    }
    i.parameters.shift()
    const tmpIntVec2 = new core.VectorInt()
    for (let j = 0; j < i.parameters[0].length; ++j) {
      tmpIntVec2.push_back(i.parameters[0][j])
    }
    i.parameters.shift()
    await outputService.createOrUpdate(job.id, i.output, fn(tmp, tmpIntVec, tmpIntVec2, ...i.parameters), oType)
    tmpIntVec.delete()
    tmpIntVec2.delete()

    return false
  }

  async single_quid_mc_double_output_op (job, fn, i, oType, core) {
    const tmp = this.validate_sid(i.parameters[0], job)
    i.parameters.shift()
    const tmpIntVec = new core.VectorInt()
    for (let j = 0; j < i.parameters[0].length; ++j) {
      tmpIntVec.push_back(i.parameters[0][j])
    }
    i.parameters.shift()
    const tmpDoubleVec = new core.VectorInt()
    for (let j = 0; j < i.parameters[0].length; ++j) {
      tmpDoubleVec.push_back(i.parameters[0][j])
    }
    i.parameters.shift()
    await outputService.createOrUpdate(job.id, i.output, fn(tmp, tmpIntVec, tmpDoubleVec, ...i.parameters), oType)
    tmpIntVec.delete()
    tmpDoubleVec.delete()

    return false
  }

  async runQrackProgram (core, p, job) {
    let tmp, tmp2, tmpIntVec, tmpCharVec, tmpDoubleVec
    for (let lcv = 0; lcv < p.length; ++lcv) {
      const i = p[lcv]
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
          this.single_quid_mc_output_op(job, core.try_separate_tol, i, 2, core)
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
          this.single_quid_mc2_output_op(job, core.perm_prob, i, 3, core)
          break
        case 'perm_prob_rdm':
          this.single_quid_mc_pauli_output_op(job, core.perm_prob_rdm, i, 3, core)
          break
        case 'fact_exp':
          this.single_quid_mc2_output_op(job, core.fact_exp, i, 3, core)
          break
        case 'fact_exp_rdm':
          this.single_quid_mc2_output_op(job, core.fact_exp_rdm, i, 3, core)
          break
        case 'fact_exp_fp':
          this.single_quid_mc_double_output_op(job, core.fact_exp_fp, i, 3, core)
          break
        case 'fact_exp_fp_rdm':
          this.single_quid_mc_double_output_op(job, core.fact_exp_fp_rdm, i, 3, core)
          break
        case 'phase_parity':
          this.single_quid_mc_op(job, core.phase_parity, i, core)
          break
        case 'joint_ensemble_prob':
          this.single_quid_mc_pauli_output_op(job, core.joint_ensemble_prob, i, 3, core)
          break
        case 'compose':
          tmp = this.validate_sid(i.parameters[0], job)
          i.parameters.shift()
          tmp2 = this.validate_sid(i.parameters[0], job)
          i.parameters.shift()
          tmpIntVec = new core.VectorInt(i.parameters[0])
          i.parameters.shift()
          core.compose(tmp, tmp2, tmpIntVec)
          break
        case 'decompose':
          this.single_quid_mc_output_op(job, core.decompose, i, 1, core)
          break
        case 'dispose':
          this.single_quid_mc_op(job, core.dispose, i, core)
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
          this.single_quid_mc_pauli_output_op(job, core.measure_basis, i, 3, core)
          break
        case 'measure_all':
          this.single_quid_output_op(job, core.measure_all, i, 4)
          break
        case 'measure_shots':
          tmp = this.validate_sid(i.parameters[0], job)
          i.parameters.shift()
          tmpIntVec = new core.VectorInt()
          for (let j = 0; j < i.parameters[0].length; ++j) {
            tmpIntVec.push_back(i.parameters[0][j])
          }
          i.parameters.shift()
          tmpCharVec = new core.VectorChar()
          for (let j = 0; j < i.parameters[0].length; ++j) {
            tmpCharVec.push_back(i.parameters[0][j])
          }
          i.parameters.shift()
          await outputService.createOrUpdate(job.id, i.output, core.measure_shots(tmp, tmpIntVec, tmpCharVec), 5)
          tmpIntVec.delete()
          tmpCharVec.delete()
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
          tmpDoubleVec = new core.VectorDouble(i.parameters[0])
          i.parameters.shift()
          core.mtrx(tmp, tmpDoubleVec, ...i.parameters)
          break
        case 'mcx':
          this.single_quid_mc_op(job, core.mcx, i, core)
          break
        case 'mcy':
          this.single_quid_mc_op(job, core.mcy, i, core)
          break
        case 'mcz':
          this.single_quid_mc_op(job, core.mcz, i, core)
          break
        case 'mch':
          this.single_quid_mc_op(job, core.mch, i, core)
          break
        case 'mcs':
          this.single_quid_mc_op(job, core.mcs, i, core)
          break
        case 'mct':
          this.single_quid_mc_op(job, core.mct, i, core)
          break
        case 'mcadjs':
          this.single_quid_mc_op(job, core.mcadjs, i, core)
          break
        case 'mcadjt':
          this.single_quid_mc_op(job, core.mcadjt, i, core)
          break
        case 'mcu':
          this.single_quid_mc_op(job, core.mcu, i, core)
          break
        case 'mcmtrx':
          this.single_quid_mc_mtrx_op(job, core.mcmtrx, i, core)
          break
        case 'macx':
          this.single_quid_mc_op(job, core.macx, i, core)
          break
        case 'macy':
          this.single_quid_mc_op(job, core.macy, i, core)
          break
        case 'macz':
          this.single_quid_mc_op(job, core.macz, i, core)
          break
        case 'mach':
          this.single_quid_mc_op(job, core.mach, i, core)
          break
        case 'macs':
          this.single_quid_mc_op(job, core.macs, i, core)
          break
        case 'mact':
          this.single_quid_mc_op(job, core.mact, i, core)
          break
        case 'macadjs':
          this.single_quid_mc_op(job, core.macadjs, i, core)
          break
        case 'macadjt':
          this.single_quid_mc_op(job, core.macadjt, i, core)
          break
        case 'macu':
          this.single_quid_mc_op(job, core.macu, i, core)
          break
        case 'macmtrx':
          this.single_quid_mc_mtrx_op(job, core.macmtrx, i, core)
          break
        case 'ucmtrx':
          this.single_quid_mc_mtrx_op(job, core.ucmtrx, i, core)
          break
        case 'multiplex_1qb_mtrx':
          this.single_quid_mc_mtrx_op(job, core.multiplex_1qb_mtrx, i, core)
          break
        case 'mx':
          this.single_quid_mc_op(job, core.mx, i, core)
          break
        case 'my':
          this.single_quid_mc_op(job, core.my, i, core)
          break
        case 'mz':
          this.single_quid_mc_op(job, core.mz, i, core)
          break
        case 'r':
          this.single_quid_op(job, core.r, i, core)
          break
        case 'mcr':
          this.single_quid_mc_op(job, core.mcr, i, core)
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

    qrack.then(async (core) => {
      return await this.runQrackProgram(core, reqBody.program, job)
    })
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

    const statusObj = await job.getJobStatusType()
    const status = {
      id: statusObj.id,
      name: statusObj.name,
      message: job.statusMessage
    }

    const outputObj = await outputService.getByJobId(jobId)
    if (!outputObj.success) {
      return outputObj
    }
    const outputArray = outputObj.body
    const output = {}
    for (let i = 0; i < outputArray.length; ++i) {
      const p = outputArray[i]
      switch (p.outputTypeId) {
        case 1:
          output[p.dataValues.name] = parseInt(p.dataValues.value)
          break
        case 2:
          output[p.dataValues.name] = (p.dataValues.value === 'true')
          break
        case 3:
          output[p.dataValues.name] = parseFloat(p.dataValues.value)
          break
        case 4:
            output[p.dataValues.name] = parseInt(p.dataValues.value)
            break
        case 5:
        default:
          output[p.dataValues.name] = p.dataValues.value
          break
      }
    }


    return { success: true, body: { status, output } }
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
