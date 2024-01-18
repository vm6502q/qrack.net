// jobService.js

// Data Access Layer
const ModelService = require('./modelService')
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

    qrack.then(async (core) => {
      const p = reqBody.program
      for (i in p) {
        switch (i.name) {
          case 'init_general':
            let sid = core.init_general(...i.parameters)
            // TODO: Call service to save sid in output, in database.
            break;
          default:
            break;
        }
      }

      // Job status 1: SUCCESS
      job.jobStatusTypeId = 1
      await job.save()
    }).error(async () => {
      // Job status 2: FAILURE
      job.jobStatusTypeId = 2
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
