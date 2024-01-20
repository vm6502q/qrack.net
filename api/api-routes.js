// api-routes.js

// Initialize ExpressJS router.
import express from 'express'
// Controllers
import accountController from './controller/accountController.js'
import qrackController from './controller/qrackController.js'
const router = express.Router()

// Set default API response.
router.get('/', function (req, res) {
  res.json({
    status: 'API is working',
    message: 'This is the Metriq public REST API.'
  })
})

// Register routes.
router.route('/register')
  .post(accountController.new)
router.route('/login')
  .post(accountController.login)
router.route('/logout')
  .get(accountController.logout)
router.route('/recover')
  .post(accountController.recover)
router.route('/password')
  .post(accountController.password)
router.route('/user/password')
  .post(accountController.update_password)
router.route('/qrack/:id')
  .get(qrackController.read)

// Export API routes.
export default router
