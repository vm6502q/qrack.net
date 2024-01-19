// qrackController.js

import { routeWrapper } from '../util/controllerUtil.js'

class QrackController {
  static async read (req, res) {
    routeWrapper(res,
      async () => {
        return { success: true, body: {} }
      },
      'Retrieved job output and status by ID.')
  }
}

export default QrackController
