// accountController.js

const { routeWrapper, sendResponse, setJwtCookie } = require('../util/controllerUtil')

// Service class
const UserService = require('../service/userService')
// Service instance
const userService = new UserService()

async function loginWrapper (res, serviceFn, successMessage) {
  try {
    // Call the service function, to perform the intended action.
    const result = await serviceFn()
    if (result.success) {
      // If successful, pass the service function result as the API response.
      const jsonResponse = { message: successMessage, data: result.body }
      // If this route should log in a web user, also generate a token and set a cookie for it.
      const token = await userService.generateJwt(result.body.id)
      setJwtCookie(res, token)
      jsonResponse.token = token
      // Success - send the API response.
      res.json(jsonResponse).end()
    } else {
      // The service function handled an error, but we can't perform the intended action.
      sendResponse(res, 400, result.error)
    }
  } catch (err) {
    // There was an unhandled exception.
    sendResponse(res, 500, err)
  }
}

class AccountController {
  // Validate the registration request and create the user model.
  static async new (req, res) {
    loginWrapper(res,
      async () => await userService.register(req.body),
      'New account created!')
  }

  // Validate the login request and log the user in.
  static async login (req, res) {
    loginWrapper(res,
      async () => await userService.login(req.body),
      'Login was successful.')
  }

  // End logged-in user session.
  static async logout (req, res) {
    // Set token to none and expire after 5 seconds.
    res.cookie('token', 'none', {
      expires: new Date(Date.now() + 5 * 1000),
      httpOnly: true
    })
    res.status(200)
      .json({ success: true, message: 'Logout was successful.' })
  }

  // Generate a new recovery UUID and email.
  static async recover (req, res) {
    routeWrapper(res,
      async () => {
        await userService.sendRecoveryEmail(req.body.user)
        return { success: true, body: '' }
      },
      'Request received.',
      0)
  }

  // Change password if UUID is valid for user.
  static async recover_password (req, res) {
    routeWrapper(res,
      async () => await userService.tryPasswordRecoveryChange(req.body),
      'Successfully changed password.',
      0)
  }

  // Change password if cookie and old password are valid.
  static async update_password (req, res) {
    loginWrapper(res,
      async () => await userService.tryPasswordChange(req.auth.id, req.body),
      'Successfully changed password.')
  }
}

module.exports = AccountController
