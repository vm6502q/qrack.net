// qrackController.js

const { routeWrapper } = require('../util/controllerUtil')

// Service
const JobService = require('../service/jobService')
const jobService = new JobService()

class QrackController {
  static async new (req, res) {
    routeWrapper(res,
      async () => {
        return await jobService.create(req.body, req.auth.id)
      },
      'Retrieved job status and output by ID.')
  }

  static async read (req, res) {
    routeWrapper(res,
      async () => {
        return await jobService.getStatusAndOutput(req.params.id, req.auth.id)
      },
      'Retrieved job status and output by ID.')
  }
}

module.exports = QrackController
