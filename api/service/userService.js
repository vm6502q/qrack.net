// userService.js

const { Op } = require('sequelize')
const config = require('../config')
// Data Access Layer
const ModelService = require('./modelService')
// Database Model
const db = require('../model/index.js')

// Password hasher
const bcrypt = require('bcrypt')
const { v4 } = require('uuid')
const jwt = require('jsonwebtoken')

const nodemailer = require('nodemailer')
const User = db.user
const saltRounds = 10

const recoveryExpirationMinutes = 30
const millisPerMinute = 60000

class UserService extends ModelService {
  constructor () {
    super(User)
  }

  sanitize (user) {
    user.passwordHash = '[REDACTED]'
    user.recoveryToken = '[REDACTED]'
    user.recoveryTokenExpiration = '[REDACTED]'

    return user
  }

  async generateJwt (userId) {
    const meta = {
      algorithm: config.api.token.algorithm,
      expiresIn: config.api.token.expiresIn
    }
    return jwt.sign({ id: userId }, config.api.token.secretKey, meta)
  }

  async getByUsername (username) {
    return await this.SequelizeServiceInstance.findOne({ usernameNormal: username.trim().toLowerCase() })
  }

  async getByEmail (email) {
    return await this.SequelizeServiceInstance.findOne({ email: email.trim().toLowerCase() })
  }

  async getByUsernameOrEmail (usernameOrEmail) {
    const usernameOrEmailNormal = usernameOrEmail.trim().toLowerCase()
    return await this.SequelizeServiceInstance.findOne({ [Op.or]: [{ usernameNormal: usernameOrEmailNormal }, { email: usernameOrEmailNormal }] })
  }

  async get (userId) {
    const user = await this.getByPk(userId)
    if (!user) {
      return { success: false, error: 'User ID not found.' }
    }

    return { success: true, body: user }
  }

  async register (reqBody) {
    const validationResult = await this.validateRegistration(reqBody)
    if (!validationResult.success) {
      return validationResult
    }

    let user = await this.SequelizeServiceInstance.new()
    user.username = reqBody.username.trim()
    user.usernameNormal = reqBody.username.trim().toLowerCase()
    user.affiliation = reqBody.affiliation ? reqBody.affiliation : ''
    user.name = reqBody.name ? reqBody.name : ''
    user.email = reqBody.email.trim().toLowerCase()
    user.passwordHash = await bcrypt.hash(reqBody.password, saltRounds)

    const result = await this.create(user)
    if (!result.success) {
      return result
    }

    user = result.body
    await user.save()

    return { success: true, body: this.sanitize(user.dataValues) }
  }

  async login (reqBody) {
    const user = await this.getByUsernameOrEmail(reqBody.username)
    if (!user) {
      return { success: false, error: 'User not found.' }
    }

    const isPasswordValid = bcrypt.compareSync(reqBody.password, user.passwordHash)
    if (!isPasswordValid) {
      return { success: false, error: 'Password incorrect.' }
    }

    return { success: true, body: this.sanitize(user) }
  }

  validateEmail (email) {
    const re = /^(([^<>()[\]\\.,;:\s@"]+(\.[^<>()[\]\\.,;:\s@"]+)*)|(".+"))@((\[[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\])|(([a-zA-Z\-0-9]+\.)+[a-zA-Z]{2,}))$/
    return re.test(email)
  }

  validatePassword (password) {
    return password && (password.length >= 12)
  }

  async validateRegistration (reqBody) {
    if (!this.validatePassword(reqBody.password)) {
      return { success: false, error: 'Password is too short.' }
    }

    if (reqBody.password !== reqBody.passwordConfirm) {
      return { success: false, error: 'Password and confirmation do not match.' }
    }

    if (!reqBody.username) {
      return { success: false, error: 'Username cannot be blank.' }
    }

    const tlUsername = reqBody.username.trim().toLowerCase()
    if (tlUsername.length === 0) {
      return { success: false, error: 'Username cannot be blank.' }
    }

    if (!reqBody.email) {
      return { success: false, error: 'Email cannot be blank.' }
    }

    const tlEmail = reqBody.email.trim().toLowerCase()

    if (tlEmail.length === 0) {
      return { success: false, error: 'Email cannot be blank.' }
    }

    if (!this.validateEmail(tlEmail)) {
      return { success: false, error: 'Invalid email format.' }
    }

    const username = await this.getByUsername(tlUsername)
    if (username) {
      return { success: false, error: 'Username already in use.' }
    }

    const emailMatch = await this.getByEmail(tlEmail)
    if (emailMatch) {
      return { success: false, error: 'Email already in use.' }
    }

    return { success: true }
  }

  async sendRecoveryEmail (usernameOrEmail) {
    if (!config.supportEmail.service) {
      console.log('Skipping email - account info not set.')
      return { success: false, error: 'Support email not available.' }
    }

    let user = await this.getByUsernameOrEmail(usernameOrEmail)
    if (!user) {
      return { success: false, error: 'User not found.' }
    }
    user = await this.getByPk(user.id)
    user.recoveryToken = v4.uuidv4().toString()
    user.recoveryTokenExpiration = new Date((new Date()).getTime() + recoveryExpirationMinutes * millisPerMinute)
    await user.save()

    const transporter = nodemailer.createTransport({
      service: config.supportEmail.service,
      auth: {
        user: config.supportEmail.account,
        pass: config.supportEmail.password
      }
    })

    const mailBody = 'Your password reset link is below: \n\n' + config.web.getUri() + '/Recover/' + encodeURIComponent(user.usernameNormal) + '/' + user.recoveryToken + '\n\n If you did not request a password reset, you can ignore this message.'

    const mailOptions = {
      from: config.supportEmail.address,
      to: user.email,
      subject: 'Password reset request',
      text: mailBody
    }

    const emailResult = await transporter.sendMail(mailOptions)
    if (emailResult.accepted && (emailResult.accepted[0] === user.email)) {
      await user.save()
      return { success: true, body: user.recoveryToken }
    } else {
      return { success: false, message: 'Could not send email.' }
    }
  }

  async tryPasswordRecoveryChange (reqBody) {
    if (!this.validatePassword(reqBody.password)) {
      return { success: false, error: 'Password is too short.' }
    }

    if (reqBody.password !== reqBody.passwordConfirm) {
      return { success: false, error: 'Password and confirmation do not match.' }
    }

    let user = await this.getByUsernameOrEmail(reqBody.username)
    if (!user) {
      return { success: false, error: 'User not found.' }
    }
    user = await this.getByPk(user.id)

    if (!user.recoveryToken || (user.recoveryToken !== reqBody.uuid.toString()) || (user.recoveryTokenExpiration < new Date())) {
      return { success: false, error: 'Supplied bad recovery token.' }
    }

    user.passwordHash = await bcrypt.hash(reqBody.password, saltRounds)
    user.recoveryToken = null
    user.recoveryTokenExpiration = null
    await user.save()

    return { success: true, body: this.sanitize(user) }
  }

  async update (userId, reqBody) {
    const user = await this.getByPk(userId)
    if (!user) {
      return { success: false, error: 'User not found.' }
    }

    if (reqBody.name !== undefined) {
      user.name = reqBody.name
    }
    if (reqBody.email !== undefined) {
      user.email = reqBody.email
    }
    if (reqBody.affiliation !== undefined) {
      user.affiliation = reqBody.affiliation
    }

    await user.save()
    return this.sanitize(user)
  }

  async tryPasswordChange (userId, reqBody) {
    if (!this.validatePassword(reqBody.password)) {
      return { success: false, error: 'Password is too short.' }
    }

    if (reqBody.password !== reqBody.passwordConfirm) {
      return { success: false, error: 'Password and confirmation do not match.' }
    }

    const user = await this.getByPk(userId)
    if (!user) {
      return { success: false, error: 'User not found.' }
    }

    const isPasswordValid = bcrypt.compareSync(reqBody.oldPassword, user.passwordHash)
    if (!isPasswordValid) {
      return { success: false, error: 'Password incorrect.' }
    }

    user.passwordHash = await bcrypt.hash(reqBody.password, saltRounds)
    user.recoveryToken = null
    user.recoveryTokenExpiration = null
    await user.save()

    return { success: true, body: this.sanitize(user) }
  }

  async delete (userId) {
    const user = await this.getByPk(userId)
    if (!user) {
      return { success: false, error: 'User not found.' }
    }

    await user.destroy()

    return { success: true, body: user }
  }
}

module.exports = UserService
