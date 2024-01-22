// qrackController.js

const { routeWrapper } = require('../util/controllerUtil')

// Service
const JobService = require('../service/jobService')
const jobService = new JobService()

class QrackController {
  static async new (req, res) {
    routeWrapper(res,
      async () => {
        return await jobService.new(req.body, 1)
      },
      'Created new job for user.')
  }

  static async read (req, res) {
    routeWrapper(res,
      async () => {
        return await jobService.getStatusAndOutput(req.params.id, 1)
      },
      'Retrieved job status and output by ID.')
  }
}

module.exports = QrackController
