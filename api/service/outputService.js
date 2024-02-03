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
    const output = await this.SequelizeServiceInstance.getByPk(outputId)
    if (!output) {
      return { success: false, error: 'Output ID not found.' }
    }

    return { success: true, body: output }
  }

  async getByJobId (jobId) {
    const output = await this.SequelizeServiceInstance.findAll({ jobId })
    if (!output) {
      return { success: false, error: 'Output ID not found.' }
    }

    return { success: true, body: output }
  }

  async getSidsByJobId (jobId) {
    return await this.SequelizeServiceInstance.findAll({ jobId, outputTypeId: 1 })
  }

  async getNidsByJobId (jobId) {
    return await this.SequelizeServiceInstance.findAll({ jobId, outputTypeId: 7 })
  }

  async getByJobIdAndName (jobId, name) {
    return await this.SequelizeServiceInstance.findOne({ jobId, name })
  }

  async createOrUpdate (job, name, value, outputTypeId) {
    if ((outputTypeId === 5) || (outputTypeId === 6)) {
      if (value.size() === 0) {
        value = ''
      } else {
        const maxLcv = value.size() - 1
        let o = ''
        for (let i = 0; i < maxLcv; ++i) {
          o = o + (value.get(i)).toString() + ','
        }
        o = o + (value.get(maxLcv)).toString()
        value = o
      }
    } else {
      value = value.toString()
    }
    let [output, isNew] = await this.SequelizeServiceInstance.findOrCreate({ jobId: job.id, name }, { jobId: job.id, outputTypeId, name, value })
    if (!isNew && (output.outputTypeId === 1) || (output.outputTypeId === 7)) {
      throw new Error('Cannot overwrite simulator or neuron quids in output space!')
    }
    output.value = value
    await output.save()

    return { success: true, body: output }
  }
}

module.exports = OutputService
