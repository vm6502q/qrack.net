// outputService.js

// Data Access Layer
const ModelService = require('./modelService')
// Database Model
const db = require('../model/index')
const Output = db.output

class OutputService extends ModelService {
  constructor () {
    super(Output)
  }

  async get (outputId) {
    const output = await this.getByPk(outputId)
    if (!output) {
      return { success: false, error: 'Output ID not found.' }
    }

    return { success: true, body: output }
  }

  async getByJobId (jobId) {
    const output = await this.findAll({ jobId })
    if (!output) {
      return { success: false, error: 'Output ID not found.' }
    }

    return { success: true, body: output }
  }

  async createOrUpdate (jobId, name, value) {
    let output = await this.findOne({ jobId, name })
    if (!output) {
        output = await this.SequelizeServiceInstance.new()
    }
    output.jobId = jobId
    output.name = name
    output.value = value

    const result = await this.create(output)
    if (!result.success) {
      return result
    }

    output = result.body
    await output.save()

    return { success: true, body: job }
  }
}

module.exports = OutputService