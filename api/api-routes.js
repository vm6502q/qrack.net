// api-routes.js

// Initialize ExpressJS router.
const express = require('express')
// Controllers
const accountController = require('./controller/accountController')
const qrackController = require('./controller/qrackController')
const router = express.Router()

// Set default API response.
router.get('/', function (req, res) {
  res.json({
    status: 'API is working',
    message: 'This is the QrackNet public REST API.'
  })
})

// Register routes.
router.route('/register')
  .post(accountController.new)
router.route('/login')
  .put(accountController.login)
router.route('/logout')
  .get(accountController.logout)
router.route('/recover')
  .post(accountController.recover)
router.route('/password')
  .post(accountController.recover_password)
router.route('/user/password')
  .post(accountController.update_password)
router.route('/qrack/:id')
  .get(qrackController.read)
router.route('/qrack')
  .get(qrackController.new)

// Export API routes.
module.exports = router
