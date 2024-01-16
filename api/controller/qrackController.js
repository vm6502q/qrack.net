// qrackController.js

const { routeWrapper } = require('../util/controllerUtil')

exports.read = async function (req, res) {
  routeWrapper(res,
    async () => {
      return { success: true, body: {} }
    },
    'Retrieved job output and status by ID.')
}
