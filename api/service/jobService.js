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

const outputService = new OutputService()
const Job = db.job

class JobService extends ModelService {
  constructor () {
    super(Job)
  }

  async failRunningJobs () {
    const jobs = await this.SequelizeServiceInstance.findAll({ jobStatusTypeId: 3 })
    for (let i = 0; i < jobs.length; ++i) {
      jobs[i].jobStatusTypeId = 2
      jobs[i].statusMessage = 'Server crashed during job (likely due to out-of-memory).'
      await jobs[i].save()
    }
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
      case 7:
        return parseInt(v.value)
      default:
        throw new Error('Quid instruction parameter does not have quid outputTypeId.')
    }
  }

  async validate_var (name, job, core, top) {
    if (top && (Symbol.iterator in Object(name))) {
      let n = []
      for (let lcv = 0; lcv < name.length; ++lcv) {
        n.push(await this.validate_var(name[lcv], job, core, false))
      }
      return n
    }
    if ((typeof name === 'boolean') || !isNaN(name)) {
      return name
    }
    const v = await outputService.getByJobIdAndName(job.id, name)
    if (!v) {
      throw new Error('Variable name not found in job output space.')
    }
    let o, valStrings
    switch (v.outputTypeId) {
      case 2:
        return v.value === 'true'
      case 3:
        return parseFloat(v.value)
      case 4:
        return parseInt(v.value)
      case 5:
        valStrings = v.value.split(',')
        o = []
        for (let i = 0; i < valStrings.length; ++i) {
          o.push(parseInt(valStrings[i]))
        }
        return o
      case 6:
        valStrings = v.value.split(',')
        o = []
        for (let i = 0; i < valStrings.length; ++i) {
          o.push(parseFloat(valStrings[i]))
        }
        return o
      case 1:
      case 7:
        throw new Error('Variable name parameter cannot have quid outputTypeId.')
      default:
        throw new Error('Unrecognized outputTypeId.')
    }
  }

  async single_quid_op (job, fn, i, core) {
    const tmp = await this.validate_sid(i.parameters[0], job)
    i.parameters.shift()
    for (let lcv = 0; lcv < i.parameters.length; ++lcv) {
      i.parameters[lcv] = await this.validate_var(i.parameters[lcv], job, core, true)
    }
    fn(tmp, ...i.parameters)
  }

  async single_quid_r_op (job, fn, i, core, b) {
    const tmp = await this.validate_sid(i.parameters[0], job)
    i.parameters.shift()
    for (let lcv = 0; lcv < i.parameters.length; ++lcv) {
      i.parameters[lcv] = await this.validate_var(i.parameters[lcv], job, core, true)
    }
    fn(tmp, ...i.parameters, b)
  }

  async single_quid_output_op (job, fn, i, oType, core) {
    if (!i.output) {
      return await this.single_quid_op(job, fn, i, core)
    }
    const tmp = await this.validate_sid(i.parameters[0], job)
    i.parameters.shift()
    for (let lcv = 0; lcv < i.parameters.length; ++lcv) {
      i.parameters[lcv] = await this.validate_var(i.parameters[lcv], job, core, true)
    }
    await outputService.createOrUpdate(job, i.output, fn(tmp, ...i.parameters), oType)
  }

  async single_quid_mc_op (job, fn, i, core) {
    const tmp = await this.validate_sid(i.parameters[0], job)
    i.parameters.shift()
    for (let lcv = 0; lcv < i.parameters.length; ++lcv) {
      i.parameters[lcv] = await this.validate_var(i.parameters[lcv], job, core, true)
    }
    if (!(Symbol.iterator in Object(i.parameters[0]))) {
      i.parameters[0] = [i.parameters[0]]
    }
    const tmpIntVec = new core.VectorInt()
    for (let j = 0; j < i.parameters[0].length; ++j) {
      tmpIntVec.push_back(i.parameters[0][j])
    }
    i.parameters.shift()
    try {
      fn(tmp, tmpIntVec, ...i.parameters)
    } catch (e) {
      tmpIntVec.delete()
      throw e
    }
    tmpIntVec.delete()
  }

  async single_quid_mc_r_op (job, fn, i, core, b) {
    const tmp = await this.validate_sid(i.parameters[0], job)
    i.parameters.shift()
    for (let lcv = 0; lcv < i.parameters.length; ++lcv) {
      i.parameters[lcv] = await this.validate_var(i.parameters[lcv], job, core, true)
    }
    if (!(Symbol.iterator in Object(i.parameters[0]))) {
      i.parameters[0] = [i.parameters[0]]
    }
    const tmpIntVec = new core.VectorInt()
    for (let j = 0; j < i.parameters[0].length; ++j) {
      tmpIntVec.push_back(i.parameters[0][j])
    }
    i.parameters.shift()
    try {
      fn(tmp, tmpIntVec, ...i.parameters, b)
    } catch (e) {
      tmpIntVec.delete()
      throw e
    }
    tmpIntVec.delete()
  }

  async single_quid_mac_r_op (job, fn, i, core, b) {
    const tmp = await this.validate_sid(i.parameters[0], job)
    i.parameters.shift()
    for (let lcv = 0; lcv < i.parameters.length; ++lcv) {
      i.parameters[lcv] = await this.validate_var(i.parameters[lcv], job, core, true)
    }
    if (!(Symbol.iterator in Object(i.parameters[0]))) {
      i.parameters[0] = [i.parameters[0]]
    }
    const tmpIntVec = new core.VectorInt()
    const cs = i.parameters[0]
    for (let j = 0; j < cs.length; ++j) {
      const c = cs[j]
      tmpIntVec.push_back(c)
      core.x(tmp, c)
    }
    i.parameters.shift()
    try {
      fn(tmp, tmpIntVec, ...i.parameters, b)
    } catch (e) {
      tmpIntVec.delete()
      throw e
    }
    tmpIntVec.delete()
    for (let j = 0; j < cs.length; ++j) {
      core.x(tmp, cs[j])
    }
  }

  async single_quid_c_r_op (job, fn, i, core, b) {
    const tmp = await this.validate_sid(i.parameters[0], job)
    i.parameters.shift()
    for (let lcv = 0; lcv < i.parameters.length; ++lcv) {
      i.parameters[lcv] = await this.validate_var(i.parameters[lcv], job, core, true)
    }
    if (!(Symbol.iterator in Object(i.parameters[0]))) {
      i.parameters[0] = [i.parameters[0]]
    }
    const tmpIntVec = new core.VectorInt()
    tmpIntVec.push_back(i.parameters[0])
    i.parameters.shift()
    try {
      fn(tmp, tmpIntVec, ...i.parameters, b)
    } catch (e) {
      tmpIntVec.delete()
      throw e
    }
    tmpIntVec.delete()
  }

  async single_quid_ac_r_op (job, fn, i, core, b) {
    const tmp = await this.validate_sid(i.parameters[0], job)
    i.parameters.shift()
    for (let lcv = 0; lcv < i.parameters.length; ++lcv) {
      i.parameters[lcv] = await this.validate_var(i.parameters[lcv], job, core, true)
    }
    if (!(Symbol.iterator in Object(i.parameters[0]))) {
      i.parameters[0] = [i.parameters[0]]
    }
    const tmpIntVec = new core.VectorInt()
    const c = i.parameters[0]
    tmpIntVec.push_back(c)
    core.x(tmp, c)
    i.parameters.shift()
    try {
      fn(tmp, tmpIntVec, ...i.parameters, b)
    } catch (e) {
      tmpIntVec.delete()
      throw e
    }
    tmpIntVec.delete()
    core.x(tmp, c)
  }

  async single_quid_c_op (job, fn, i, core) {
    const tmp = await this.validate_sid(i.parameters[0], job)
    i.parameters.shift()
    for (let lcv = 0; lcv < i.parameters.length; ++lcv) {
      i.parameters[lcv] = await this.validate_var(i.parameters[lcv], job, core, true)
    }
    if (!(Symbol.iterator in Object(i.parameters[0]))) {
      i.parameters[0] = [i.parameters[0]]
    }
    const tmpIntVec = new core.VectorInt()
    tmpIntVec.push_back(i.parameters[0])
    i.parameters.shift()
    try {
      fn(tmp, tmpIntVec, ...i.parameters)
    } catch (e) {
      tmpIntVec.delete()
      throw e
    }
    tmpIntVec.delete()
  }

  async single_quid_mc_output_op (job, fn, i, oType, core) {
    if (!i.output) {
      return await this.single_quid_mc_op(job, fn, i, core)
    }
    const tmp = await this.validate_sid(i.parameters[0], job)
    i.parameters.shift()
    for (let lcv = 0; lcv < i.parameters.length; ++lcv) {
      i.parameters[lcv] = await this.validate_var(i.parameters[lcv], job, core, true)
    }
    if (!(Symbol.iterator in Object(i.parameters[0]))) {
      i.parameters[0] = [i.parameters[0]]
    }
    const tmpIntVec = new core.VectorInt()
    for (let j = 0; j < i.parameters[0].length; ++j) {
      tmpIntVec.push_back(i.parameters[0][j])
    }
    i.parameters.shift()
    try {
      await outputService.createOrUpdate(job, i.output, fn(tmp, tmpIntVec, ...i.parameters), oType)
    } catch (e) {
      tmpIntVec.delete()
      throw e
    }
    tmpIntVec.delete()
  }

  async single_quid_mc_pauli_output_op (job, fn, i, oType, core) {
    const tmp = await this.validate_sid(i.parameters[0], job)
    i.parameters.shift()
    for (let lcv = 0; lcv < i.parameters.length; ++lcv) {
      i.parameters[lcv] = await this.validate_var(i.parameters[lcv], job, core, true)
    }
    if (!(Symbol.iterator in Object(i.parameters[0]))) {
      i.parameters[0] = [i.parameters[0]]
    }
    const tmpIntVec = new core.VectorInt()
    for (let j = 0; j < i.parameters[0].length; ++j) {
      tmpIntVec.push_back(i.parameters[0][j])
    }
    i.parameters.shift()
    if (!(Symbol.iterator in Object(i.parameters[0]))) {
      i.parameters[0] = [i.parameters[0]]
    }
    const tmpCharVec = new core.VectorChar()
    for (let j = 0; j < i.parameters[0].length; ++j) {
      tmpCharVec.push_back(i.parameters[0][j])
    }
    i.parameters.shift()
    try {
      if (i.output) {
        await outputService.createOrUpdate(job, i.output, fn(tmp, tmpIntVec, tmpCharVec, ...i.parameters), oType)
      } else {
        fn(tmp, tmpIntVec, tmpCharVec, ...i.parameters)
      }
    } catch (e) {
      tmpIntVec.delete()
      tmpCharVec.delete()
      throw e
    }
    tmpIntVec.delete()
    tmpCharVec.delete()
  }

  async single_quid_mc_mtrx_op (job, fn, i, core) {
    const tmp = await this.validate_sid(i.parameters[0], job)
    i.parameters.shift()
    for (let lcv = 0; lcv < i.parameters.length; ++lcv) {
      i.parameters[lcv] = await this.validate_var(i.parameters[lcv], job, core, true)
    }
    if (!(Symbol.iterator in Object(i.parameters[0]))) {
      i.parameters[0] = [i.parameters[0]]
    }
    const tmpIntVec = new core.VectorInt()
    for (let j = 0; j < i.parameters[0].length; ++j) {
      tmpIntVec.push_back(i.parameters[0][j])
    }
    if (!(Symbol.iterator in Object(i.parameters[0]))) {
      i.parameters[0] = [i.parameters[0]]
    }
    i.parameters.shift()
    const tmpDoubleVec = new core.VectorInt()
    for (let j = 0; j < i.parameters[0].length; ++j) {
      tmpDoubleVec.push_back(i.parameters[0][j])
    }
    i.parameters.shift()
    try {
      fn(tmp, tmpIntVec, tmpDoubleVec, ...i.parameters)
    } catch (e) {
      tmpIntVec.delete()
      tmpDoubleVec.delete()
      throw e
    }
    tmpIntVec.delete()
    tmpDoubleVec.delete()
  }

  async single_quid_c_mtrx_op (job, fn, i, core) {
    const tmp = await this.validate_sid(i.parameters[0], job)
    i.parameters.shift()
    for (let lcv = 0; lcv < i.parameters.length; ++lcv) {
      i.parameters[lcv] = await this.validate_var(i.parameters[lcv], job, core, true)
    }
    if (!(Symbol.iterator in Object(i.parameters[0]))) {
      i.parameters[0] = [i.parameters[0]]
    }
    const tmpIntVec = new core.VectorInt()
    tmpIntVec.push_back(i.parameters[0])
    i.parameters.shift()
    if (!(Symbol.iterator in Object(i.parameters[0]))) {
      i.parameters[0] = [i.parameters[0]]
    }
    const tmpDoubleVec = new core.VectorInt()
    for (let j = 0; j < i.parameters[0].length; ++j) {
      tmpDoubleVec.push_back(i.parameters[0][j])
    }
    i.parameters.shift()
    try {
      fn(tmp, tmpIntVec, tmpDoubleVec, ...i.parameters)
    } catch (e) {
      tmpIntVec.delete()
      tmpDoubleVec.delete()
      throw e
    }
    tmpIntVec.delete()
    tmpDoubleVec.delete()
  }

  async single_quid_mc2_op (job, fn, i, core) {
    const tmp = await this.validate_sid(i.parameters[0], job)
    i.parameters.shift()
    for (let lcv = 0; lcv < i.parameters.length; ++lcv) {
      i.parameters[lcv] = await this.validate_var(i.parameters[lcv], job, core, true)
    }
    if (!(Symbol.iterator in Object(i.parameters[0]))) {
      i.parameters[0] = [i.parameters[0]]
    }
    const tmpIntVec = new core.VectorInt()
    for (let j = 0; j < i.parameters[0].length; ++j) {
      tmpIntVec.push_back(i.parameters[0][j])
    }
    i.parameters.shift()
    if (!(Symbol.iterator in Object(i.parameters[0]))) {
      i.parameters[0] = [i.parameters[0]]
    }
    const tmpIntVec2 = new core.VectorInt()
    for (let j = 0; j < i.parameters[0].length; ++j) {
      tmpIntVec2.push_back(i.parameters[0][j])
    }
    i.parameters.shift()
    try {
      fn(tmp, tmpIntVec, tmpIntVec2, ...i.parameters)
    } catch (e) {
      tmpIntVec.delete()
      tmpIntVec2.delete()
      throw e
    }
    tmpIntVec.delete()
    tmpIntVec2.delete()
  }

  async single_quid_mc2_output_op (job, fn, i, oType, core) {
    if (!i.output) {
      return await this.single_quid_mc2_op(job, fn, i, core)
    }
    const tmp = await this.validate_sid(i.parameters[0], job)
    i.parameters.shift()
    for (let lcv = 0; lcv < i.parameters.length; ++lcv) {
      i.parameters[lcv] = await this.validate_var(i.parameters[lcv], job, core, true)
    }
    if (!(Symbol.iterator in Object(i.parameters[0]))) {
      i.parameters[0] = [i.parameters[0]]
    }
    const tmpIntVec = new core.VectorInt()
    for (let j = 0; j < i.parameters[0].length; ++j) {
      tmpIntVec.push_back(i.parameters[0][j])
    }
    i.parameters.shift()
    if (!(Symbol.iterator in Object(i.parameters[0]))) {
      i.parameters[0] = [i.parameters[0]]
    }
    const tmpIntVec2 = new core.VectorInt()
    for (let j = 0; j < i.parameters[0].length; ++j) {
      tmpIntVec2.push_back(i.parameters[0][j])
    }
    i.parameters.shift()
    try {
      await outputService.createOrUpdate(job, i.output, fn(tmp, tmpIntVec, tmpIntVec2, ...i.parameters), oType)
    } catch (e) {
      tmpIntVec.delete()
      tmpIntVec2.delete()
      throw e
    }
    tmpIntVec.delete()
    tmpIntVec2.delete()
  }

  async single_quid_mc3_op (job, fn, i, core) {
    const tmp = await this.validate_sid(i.parameters[0], job)
    i.parameters.shift()
    for (let lcv = 0; lcv < i.parameters.length; ++lcv) {
      i.parameters[lcv] = await this.validate_var(i.parameters[lcv], job, core, true)
    }
    if (!(Symbol.iterator in Object(i.parameters[0]))) {
      i.parameters[0] = [i.parameters[0]]
    }
    const tmpIntVec = new core.VectorInt()
    for (let j = 0; j < i.parameters[0].length; ++j) {
      tmpIntVec.push_back(i.parameters[0][j])
    }
    i.parameters.shift()
    if (!(Symbol.iterator in Object(i.parameters[0]))) {
      i.parameters[0] = [i.parameters[0]]
    }
    const tmpIntVec2 = new core.VectorInt()
    for (let j = 0; j < i.parameters[0].length; ++j) {
      tmpIntVec2.push_back(i.parameters[0][j])
    }
    i.parameters.shift()
    if (!(Symbol.iterator in Object(i.parameters[0]))) {
      i.parameters[0] = [i.parameters[0]]
    }
    const tmpIntVec3 = new core.VectorInt()
    for (let j = 0; j < i.parameters[0].length; ++j) {
      tmpIntVec3.push_back(i.parameters[0][j])
    }
    i.parameters.shift()
    try {
      fn(tmp, tmpIntVec, tmpIntVec2, tmpIntVec3, ...i.parameters)
    } catch (e) {
      tmpIntVec.delete()
      tmpIntVec2.delete()
      tmpIntVec3.delete()
      throw e
    }
    tmpIntVec.delete()
    tmpIntVec2.delete()
    tmpIntVec3.delete()
  }

  async single_quid_mc_double_output_op (job, fn, i, oType, core) {
    const tmp = await this.validate_sid(i.parameters[0], job)
    i.parameters.shift()
    for (let lcv = 0; lcv < i.parameters.length; ++lcv) {
      i.parameters[lcv] = await this.validate_var(i.parameters[lcv], job, core, true)
    }
    if (!(Symbol.iterator in Object(i.parameters[0]))) {
      i.parameters[0] = [i.parameters[0]]
    }
    const tmpIntVec = new core.VectorInt()
    for (let j = 0; j < i.parameters[0].length; ++j) {
      tmpIntVec.push_back(i.parameters[0][j])
    }
    i.parameters.shift()
    if (!(Symbol.iterator in Object(i.parameters[0]))) {
      i.parameters[0] = [i.parameters[0]]
    }
    const tmpDoubleVec = new core.VectorInt()
    for (let j = 0; j < i.parameters[0].length; ++j) {
      tmpDoubleVec.push_back(i.parameters[0][j])
    }
    i.parameters.shift()
    try {
      if (i.output) {
        await outputService.createOrUpdate(job, i.output, fn(tmp, tmpIntVec, tmpDoubleVec, ...i.parameters), oType)
      } else {
        fn(tmp, tmpIntVec, tmpDoubleVec, ...i.parameters)
      }
    } catch (e) {
      tmpIntVec.delete()
      tmpDoubleVec.delete()
      throw e
    }
    tmpIntVec.delete()
    tmpDoubleVec.delete()
  }

  async runQrackProgram (core, p, job, isSub) {
    let tmp, tmp2, tmpIntVec, tmpIntVec2, tmpCharVec, tmpDoubleVec
    for (let lcv = 0; lcv < p.length; ++lcv) {
      const i = p[lcv]
      if (!(Symbol.iterator in Object(i.parameters))) {
        i.parameters = [i.parameters]
      }
      switch (i.name) {
        case 'not':
          tmp = i.parameters.pop()
          tmp2 = await this.validate_var(tmp, job, core, true)
          await outputService.createOrUpdate(job, tmp, !tmp2, 2)
          break
        case 'cif':
          tmp = await this.validate_var(i.parameters.pop(), job, core, true)
          if (tmp) {
            if (!(Symbol.iterator in Object(i.program))) {
              i.program = [i.program]
            }
            await this.runQrackProgram(core, i.program, job, true)
          }
          break
        case 'for':
          tmp = parseInt(i.parameters[0])
          if (isNaN(tmp)) {
            tmp = await this.validate_var(tmp, job, core, true)
          }
          if (!(Symbol.iterator in Object(i.program))) {
            i.program = [i.program]
          }
          for (let _lcv = 0; _lcv < tmp; ++_lcv) {
            const p = structuredClone(i.program)
            if (i.output) {
                await outputService.createOrUpdate(job, i.output, _lcv, 4)
            }
            await this.runQrackProgram(core, p, job, true)
          }
          if (i.output) {
              await outputService.createOrUpdate(job, i.output, tmp, 4)
          }
          break
        case 'set_permutation':
          tmp = await this.validate_sid(i.parameters[0], job)
          tmp2 = isNaN(i.parameters[1]) ? await this.validate_var(i.parameters[1], job, core, true) : i.parameters[1]
          core.set_permutation(tmp, tmp2)
          break
        case 'write_bool':
          await outputService.createOrUpdate(job, i.output, !!(i.parameters[0]), 2)
          break
        case 'init_general':
          await outputService.createOrUpdate(job, i.output, core.init_general(...i.parameters), 1)
          break
        case 'init_stabilizer':
          await outputService.createOrUpdate(job, i.output, core.init_stabilizer(...i.parameters), 1)
          break
        case 'init_qbdd':
          await outputService.createOrUpdate(job, i.output, core.init_qbdd(...i.parameters), 1)
          break
        case 'init_clone':
          await this.single_quid_output_op(job, core.init_clone, i, 1, core)
          break
        case 'destroy':
          await this.single_quid_op(job, core.destroy, i, core)
          break
        case 'seed':
          await this.single_quid_op(job, core.seed, i, core)
          break
        case 'try_separate_1qb':
          await this.single_quid_output_op(job, core.try_separate_1qb, i, 2, core)
          break
        case 'try_separate_2qb':
          await this.single_quid_output_op(job, core.try_separate_2qb, i, 2, core)
          break
        case 'try_separate_tol':
          await this.single_quid_mc_output_op(job, core.try_separate_tol, i, 2, core)
          break
        case 'get_unitary_fidelity':
          await this.single_quid_output_op(job, core.get_unitary_fidelity, i, 3, core)
          break
        case 'reset_unitary_fidelity':
          await this.single_quid_op(job, core.reset_unitary_fidelity, i, core)
          break
        case 'set_sdrp':
          await this.single_quid_op(job, core.set_sdrp, i, core)
          break
        case 'set_ncrp':
          await this.single_quid_op(job, core.set_ncrp, i, core)
          break
        case 'set_reactive_separate':
          await this.single_quid_op(job, core.set_reactive_separate, i, core)
          break
        case 'set_t_injection':
          await this.single_quid_op(job, core.set_t_injection, i, core)
          break
        case 'prob':
          await this.single_quid_output_op(job, core.prob, i, 3, core)
          break
        case 'prob_rdm':
          await this.single_quid_output_op(job, core.prob_rdm, i, 3, core)
          break
        case 'perm_prob':
          await this.single_quid_mc2_output_op(job, core.perm_prob, i, 3, core)
          break
        case 'perm_prob_rdm':
          await this.single_quid_mc_pauli_output_op(job, core.perm_prob_rdm, i, 3, core)
          break
        case 'fact_exp':
          await this.single_quid_mc2_output_op(job, core.fact_exp, i, 3, core)
          break
        case 'fact_exp_rdm':
          await this.single_quid_mc2_output_op(job, core.fact_exp_rdm, i, 3, core)
          break
        case 'fact_exp_fp':
          await this.single_quid_mc_double_output_op(job, core.fact_exp_fp, i, 3, core)
          break
        case 'fact_exp_fp_rdm':
          await this.single_quid_mc_double_output_op(job, core.fact_exp_fp_rdm, i, 3, core)
          break
        case 'phase_parity':
          await this.single_quid_mc_op(job, core.phase_parity, i, core)
          break
        case 'joint_ensemble_prob':
          await this.single_quid_mc_pauli_output_op(job, core.joint_ensemble_prob, i, 3, core)
          break
        case 'compose':
          tmp = await this.validate_sid(i.parameters[0], job)
          i.parameters.shift()
          tmp2 = await this.validate_sid(i.parameters[0], job)
          i.parameters.shift()
          if (!(Symbol.iterator in Object(i.parameters))) {
            i.parameters = [i.parameters]
          }
          for (let _lcv = 0; _lcv < i.parameters.length; ++_lcv) {
            i.parameters[_lcv] = await this.validate_var(i.parameters[_lcv], job, core, true)
          }
          tmpIntVec = new core.VectorInt(i.parameters[0])
          i.parameters.shift()
          try {
            core.compose(tmp, tmp2, tmpIntVec)
          } catch (e) {
            tmpIntVec.delete()
            throw e
          }
          tmpIntVec.delete()
          break
        case 'decompose':
          await this.single_quid_mc_output_op(job, core.decompose, i, 1, core)
          break
        case 'dispose':
          await this.single_quid_mc_op(job, core.dispose, i, core)
          break
        case 'reset_all':
          await this.single_quid_op(job, core.reset_all, i, core)
          break
        case 'measure':
          await this.single_quid_output_op(job, core.measure, i, 2, core)
          break
        case 'force_measure':
          await this.single_quid_op(job, core.force_measure, i, core)
          break
        case 'measure_basis':
          await this.single_quid_mc_pauli_output_op(job, core.measure_basis, i, 3, core)
          break
        case 'measure_all':
          await this.single_quid_output_op(job, core.measure_all, i, 4, core)
          break
        case 'measure_shots':
          await this.single_quid_mc_output_op(job, core.measure_shots, i, 5, core)
          break
        case 'x':
          await this.single_quid_op(job, core.x, i, core)
          break
        case 'y':
          await this.single_quid_op(job, core.y, i, core)
          break
        case 'z':
          await this.single_quid_op(job, core.z, i, core)
          break
        case 'h':
          await this.single_quid_op(job, core.h, i, core)
          break
        case 's':
          await this.single_quid_op(job, core.s, i, core)
          break
        case 't':
          await this.single_quid_op(job, core.t, i, core)
          break
        case 'adjs':
          await this.single_quid_op(job, core.adjs, i, core)
          break
        case 'adjt':
          await this.single_quid_op(job, core.adjt, i, core)
          break
        case 'u':
          await this.single_quid_op(job, core.u, i, core)
          break
        case 'mtrx':
          tmp = await this.validate_sid(i.parameters[0], job)
          i.parameters.shift()
          for (let _lcv = 0; _lcv < i.parameters.length; ++_lcv) {
            i.parameters[_lcv] = await this.validate_var(i.parameters[_lcv], job, core, true)
          }
          if (!(Symbol.iterator in Object(i.parameters[0]))) {
            i.parameters[0] = [i.parameters[0]]
          }
          tmpDoubleVec = new core.VectorDouble(i.parameters[0])
          i.parameters.shift()
          try {
            core.mtrx(tmp, tmpDoubleVec, ...i.parameters)
          } catch (e) {
            tmpDoubleVec.delete()
            throw e
          }
          tmpDoubleVec.delete()
          break
        case 'mcx':
          await this.single_quid_mc_op(job, core.mcx, i, core)
          break
        case 'mcy':
          await this.single_quid_mc_op(job, core.mcy, i, core)
          break
        case 'mcz':
          await this.single_quid_mc_op(job, core.mcz, i, core)
          break
        case 'mch':
          await this.single_quid_mc_op(job, core.mch, i, core)
          break
        case 'mcs':
          await this.single_quid_mc_op(job, core.mcs, i, core)
          break
        case 'mct':
          await this.single_quid_mc_op(job, core.mct, i, core)
          break
        case 'mcadjs':
          await this.single_quid_mc_op(job, core.mcadjs, i, core)
          break
        case 'mcadjt':
          await this.single_quid_mc_op(job, core.mcadjt, i, core)
          break
        case 'mcu':
          await this.single_quid_mc_op(job, core.mcu, i, core)
          break
        case 'mcmtrx':
          await this.single_quid_mc_mtrx_op(job, core.mcmtrx, i, core)
          break
        case 'macx':
          await this.single_quid_mc_op(job, core.macx, i, core)
          break
        case 'macy':
          await this.single_quid_mc_op(job, core.macy, i, core)
          break
        case 'macz':
          await this.single_quid_mc_op(job, core.macz, i, core)
          break
        case 'mach':
          await this.single_quid_mc_op(job, core.mach, i, core)
          break
        case 'macs':
          await this.single_quid_mc_op(job, core.macs, i, core)
          break
        case 'mact':
          await this.single_quid_mc_op(job, core.mact, i, core)
          break
        case 'macadjs':
          await this.single_quid_mc_op(job, core.macadjs, i, core)
          break
        case 'macadjt':
          await this.single_quid_mc_op(job, core.macadjt, i, core)
          break
        case 'macu':
          await this.single_quid_mc_op(job, core.macu, i, core)
          break
        case 'macmtrx':
          await this.single_quid_mc_mtrx_op(job, core.macmtrx, i, core)
          break
        case 'cx':
          await this.single_quid_c_op(job, core.mcx, i, core)
          break
        case 'cy':
          await this.single_quid_c_op(job, core.mcy, i, core)
          break
        case 'cz':
          await this.single_quid_c_op(job, core.mcz, i, core)
          break
        case 'ch':
          await this.single_quid_c_op(job, core.mch, i, core)
          break
        case 'cs':
          await this.single_quid_c_op(job, core.mcs, i, core)
          break
        case 'ct':
          await this.single_quid_c_op(job, core.mct, i, core)
          break
        case 'cadjs':
          await this.single_quid_c_op(job, core.mcadjs, i, core)
          break
        case 'cadjt':
          await this.single_quid_c_op(job, core.mcadjt, i, core)
          break
        case 'cu':
          await this.single_quid_c_op(job, core.mcu, i, core)
          break
        case 'cmtrx':
          await this.single_quid_c_mtrx_op(job, core.mcmtrx, i, core)
          break
        case 'acx':
          await this.single_quid_c_op(job, core.macx, i, core)
          break
        case 'acy':
          await this.single_quid_c_op(job, core.macy, i, core)
          break
        case 'acz':
          await this.single_quid_c_op(job, core.macz, i, core)
          break
        case 'ach':
          await this.single_quid_c_op(job, core.mach, i, core)
          break
        case 'acs':
          await this.single_quid_c_op(job, core.macs, i, core)
          break
        case 'act':
          await this.single_quid_c_op(job, core.mact, i, core)
          break
        case 'acadjs':
          await this.single_quid_c_op(job, core.macadjs, i, core)
          break
        case 'acadjt':
          await this.single_quid_c_op(job, core.macadjt, i, core)
          break
        case 'acu':
          await this.single_quid_c_op(job, core.macu, i, core)
          break
        case 'acmtrx':
          await this.single_quid_c_mtrx_op(job, core.macmtrx, i, core)
          break
        case 'ucmtrx':
          await this.single_quid_mc_mtrx_op(job, core.ucmtrx, i, core)
          break
        case 'multiplex_1qb_mtrx':
          await this.single_quid_mc_mtrx_op(job, core.multiplex_1qb_mtrx, i, core)
          break
        case 'mx':
          await this.single_quid_mc_op(job, core.mx, i, core)
          break
        case 'my':
          await this.single_quid_mc_op(job, core.my, i, core)
          break
        case 'mz':
          await this.single_quid_mc_op(job, core.mz, i, core)
          break
        case 'r':
          await this.single_quid_op(job, core.r, i, core)
          break
        case 'rx':
          await this.single_quid_r_op(job, core.r, i, core, 1)
          break
        case 'ry':
          await this.single_quid_r_op(job, core.r, i, core, 3)
          break
        case 'rz':
          await this.single_quid_r_op(job, core.r, i, core, 2)
          break
        case 'mcr':
          await this.single_quid_mc_op(job, core.mcr, i, core)
          break
        case 'mcrx':
          await this.single_quid_mc_r_op(job, core.mcr, i, core, 1)
          break
        case 'mcry':
          await this.single_quid_mc_r_op(job, core.mcr, i, core, 3)
          break
        case 'mcrz':
          await this.single_quid_mc_r_op(job, core.mcr, i, core, 2)
          break
        case 'crx':
          await this.single_quid_c_r_op(job, core.mcr, i, core, 1)
          break
        case 'cry':
          await this.single_quid_c_r_op(job, core.mcr, i, core, 3)
          break
        case 'crz':
          await this.single_quid_c_r_op(job, core.mcr, i, core, 2)
          break
        case 'macrx':
          await this.single_quid_mac_r_op(job, core.mcr, i, core, 1)
          break
        case 'macry':
          await this.single_quid_mac_r_op(job, core.mcr, i, core, 3)
          break
        case 'macrz':
          await this.single_quid_mac_r_op(job, core.mcr, i, core, 2)
          break
        case 'acrx':
          await this.single_quid_ac_r_op(job, core.mcr, i, core, 1)
          break
        case 'acry':
          await this.single_quid_ac_r_op(job, core.mcr, i, core, 3)
          break
        case 'acrz':
          await this.single_quid_ac_r_op(job, core.mcr, i, core, 2)
          break
        case 'exp':
          tmp = await this.validate_sid(i.parameters[0], job)
          i.parameters.shift()
          for (let _lcv = 0; _lcv < i.parameters.length; ++_lcv) {
            i.parameters[_lcv] = await this.validate_var(i.parameters[_lcv], job, core, true)
          }
          if (!(Symbol.iterator in Object(i.parameters[0]))) {
            i.parameters[0] = [i.parameters[0]]
          }
          tmpIntVec = new core.VectorInt()
          for (let j = 0; j < i.parameters[0].length; ++j) {
            tmpIntVec.push_back(i.parameters[0][j])
          }
          i.parameters.shift()
          if (!(Symbol.iterator in Object(i.parameters[0]))) {
            i.parameters[0] = [i.parameters[0]]
          }
          tmpCharVec = new core.VectorChar()
          for (let j = 0; j < i.parameters[0].length; ++j) {
            tmpCharVec.push_back(i.parameters[0][j])
          }
          i.parameters.shift()
          try {
            core.exp(tmp, tmpIntVec, tmpCharVec, ...i.parameters)
          } catch (e) {
            tmpIntVec.delete()
            tmpCharVec.delete()
            throw e
          }
          tmpIntVec.delete()
          tmpCharVec.delete()
          break
        case 'mcexp':
          tmp = await this.validate_sid(i.parameters[0], job)
          i.parameters.shift()
          for (let _lcv = 0; _lcv < i.parameters.length; ++_lcv) {
            i.parameters[_lcv] = await this.validate_var(i.parameters[_lcv], job, core, true)
          }
          if (!(Symbol.iterator in Object(i.parameters[0]))) {
            i.parameters[0] = [i.parameters[0]]
          }
          tmpIntVec = new core.VectorInt()
          for (let j = 0; j < i.parameters[0].length; ++j) {
            tmpIntVec.push_back(i.parameters[0][j])
          }
          i.parameters.shift()
          if (!(Symbol.iterator in Object(i.parameters[0]))) {
            i.parameters[0] = [i.parameters[0]]
          }
          tmpIntVec2 = new core.VectorInt()
          for (let j = 0; j < i.parameters[0].length; ++j) {
            tmpIntVec2.push_back(i.parameters[0][j])
          }
          i.parameters.shift()
          if (!(Symbol.iterator in Object(i.parameters[0]))) {
            i.parameters[0] = [i.parameters[0]]
          }
          tmpCharVec = new core.VectorChar()
          for (let j = 0; j < i.parameters[0].length; ++j) {
            tmpCharVec.push_back(i.parameters[0][j])
          }
          i.parameters.shift()
          try {
            core.mcexp(tmp, tmpIntVec, tmpIntVec2, tmpCharVec, ...i.parameters)
          } catch (e) {
            tmpIntVec.delete()
            tmpIntVec2.delete()
            tmpCharVec.delete()
            throw e
          }
          tmpIntVec.delete()
          tmpIntVec2.delete()
          tmpCharVec.delete()
          break
        case 'swap':
          await this.single_quid_op(job, core.swap, i, core)
          break
        case 'iswap':
          await this.single_quid_op(job, core.iswap, i, core)
          break
        case 'adjiswap':
          await this.single_quid_op(job, core.adjiswap, i, core)
          break
        case 'fsim':
          await this.single_quid_op(job, core.fsim, i, core)
          break
        case 'mcswap':
          await this.single_quid_mc_op(job, core.mcswap, i, core)
          break
        case 'macswap':
          await this.single_quid_mc_op(job, core.macswap, i, core)
          break
        case 'and':
          await this.single_quid_op(job, core.and, i, core)
          break
        case 'or':
          await this.single_quid_op(job, core.or, i, core)
          break
        case 'xor':
          await this.single_quid_op(job, core.xor, i, core)
          break
        case 'nand':
          await this.single_quid_op(job, core.nand, i, core)
          break
        case 'nor':
          await this.single_quid_op(job, core.nor, i, core)
          break
        case 'xnor':
          await this.single_quid_op(job, core.xnor, i, core)
          break
        case 'cland':
          await this.single_quid_op(job, core.cland, i, core)
          break
        case 'clor':
          await this.single_quid_op(job, core.clor, i, core)
          break
        case 'clxor':
          await this.single_quid_op(job, core.clxor, i, core)
          break
        case 'clnand':
          await this.single_quid_op(job, core.clnand, i, core)
          break
        case 'clnor':
          await this.single_quid_op(job, core.clnor, i, core)
          break
        case 'clxnor':
          await this.single_quid_op(job, core.clxnor, i, core)
          break
        case 'qft':
          await this.single_quid_mc_op(job, core.qft, i, core)
          break
        case 'iqft':
          await this.single_quid_mc_op(job, core.iqft, i, core)
          break
        case 'add':
          await this.single_quid_mc_op(job, core.add, i, core)
          break
        case 'sub':
          await this.single_quid_mc_op(job, core.sub, i, core)
          break
        case 'adds':
          await this.single_quid_mc_op(job, core.adds, i, core)
          break
        case 'subs':
          await this.single_quid_mc_op(job, core.subs, i, core)
          break
        case 'mcadd':
          await this.single_quid_mc2_op(job, core.mcadd, i, core)
          break
        case 'mcsub':
          await this.single_quid_mc2_op(job, core.mcsub, i, core)
          break
        case 'mul':
          await this.single_quid_mc2_op(job, core.mul, i, core)
          break
        case 'div':
          await this.single_quid_mc2_op(job, core.div, i, core)
          break
        case 'muln':
          await this.single_quid_mc2_op(job, core.muln, i, core)
          break
        case 'divn':
          await this.single_quid_mc2_op(job, core.divn, i, core)
          break
        case 'pown':
          await this.single_quid_mc2_op(job, core.pown, i, core)
          break
        case 'mcmul':
          await this.single_quid_mc3_op(job, core.mcmul, i, core)
          break
        case 'mcdiv':
          await this.single_quid_mc3_op(job, core.mcdiv, i, core)
          break
        case 'mcmuln':
          await this.single_quid_mc3_op(job, core.mcmuln, i, core)
          break
        case 'mcdivn':
          await this.single_quid_mc3_op(job, core.mcdivn, i, core)
          break
        case 'mcpown':
          await this.single_quid_mc3_op(job, core.mcpown, i, core)
          break
        case 'init_qneuron':
          await this.single_quid_mc_output_op(job, core.init_qneuron, i, 7, core)
          break
        case 'clone_qneuron':
          await this.single_quid_output_op(job, core.clone_qneuron, i, 7, core)
          break
        case 'destroy_qneuron':
          await this.single_quid_op(job, core.destroy_qneuron, i, core)
          break
        case 'set_qneuron_angles':
          tmp = await this.validate_sid(i.parameters[0], job)
          i.parameters.shift()
          for (let _lcv = 0; _lcv < i.parameters.length; ++_lcv) {
            i.parameters[_lcv] = await this.validate_var(i.parameters[_lcv], job, core, true)
          }
          if (!(Symbol.iterator in Object(i.parameters[0]))) {
            i.parameters[0] = [i.parameters[0]]
          }
          tmpDoubleVec = new core.VectorDouble()
          for (let j = 0; j < i.parameters[0].length; ++j) {
            tmpDoubleVec.push_back(i.parameters[0][j])
          }
          i.parameters.shift()
          try {
            core.set_qneuron_angles(tmp, tmpDoubleVec, ...i.parameters)
          } catch (e) {
            tmpDoubleVec.delete()
            throw e
          }
          tmpDoubleVec.delete()
          break
        case 'get_qneuron_angles':
          await this.single_quid_output_op(job, core.get_qneuron_angles, i, 6, core)
          break
        case 'set_qneuron_alpha':
          await this.single_quid_op(job, core.set_qneuron_alpha, i, core)
          break
        case 'set_qneuron_activation_fn':
          await this.single_quid_op(job, core.set_qneuron_activation_fn, i, core)
          break
        case 'qneuron_predict':
          await this.single_quid_end_bool2_output_op(job, core.qneuron_predict, i, 3, core)
          break
        case 'qneuron_unpredict':
          await this.single_quid_output_op(job, core.qneuron_unpredict, i, 3, core)
          break
        case 'qneuron_learn_cycle':
          await this.single_quid_output_op(job, core.qneuron_learn_cycle, i, 3, core)
          break
        case 'qneuron_learn':
          await this.single_quid_op(job, core.qneuron_learn, i, core)
          break
        case 'qneuron_learn_permutation':
          await this.single_quid_op(job, core.qneuron_learn_permutation, i, core)
          break
        default:
          throw new Error('One or more of your job program operation line names do not match a defined operation name.')
      }
    }

    if (!isSub) {
      // Job status 1: SUCCESS
      job.jobStatusTypeId = 1
      job.statusMessage = 'Job completed fully and normally.'
      await job.save()
    }
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
    job.statusMessage = 'RUNNING'

    const result = await this.create(job)
    if (!result.success) {
      return result
    }

    job = result.body
    await job.save()

    Qrack({
      locateFile: () => {
        return path.resolve(__dirname, '../Qrack.wasm')
      }
    }).catch((e) => {
      console.log('Could not load Qrack: ' + e.toString())
    }).then(async (core) => {
      await this.runQrackProgram(core, reqBody.program, job, false)
    }).catch(async (e) => {
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
      let valStrings, o
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
          valStrings = p.dataValues.value.split(',')
          o = []
          for (let i = 0; i < valStrings.length; ++i) {
            o.push(parseInt(valStrings[i]))
          }
          output[p.dataValues.name] = o
          break
        case 6:
          valStrings = p.dataValues.value.split(',')
          o = []
          for (let i = 0; i < valStrings.length; ++i) {
            o.push(parseFloat(valStrings[i]))
          }
          output[p.dataValues.name] = o
          break
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
