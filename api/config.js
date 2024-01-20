const crypto = require('crypto')

const config = {}

config.isDebug = true

config.app = { port: process.env.PORT || 8080 }

config.api = {
  url: config.isDebug ? 'localhost:8080' : 'qrack.net',
  protocol: config.isDebug ? 'http://' : 'https://',
  endpoint: '/api',
  getUriPrefix: () => { return config.api.protocol + config.api.url + config.api.endpoint }
}

config.api.token = {
  // NEVER store a valid secret key in files that might be checked into source code repositories!!!
  secretKey: process.env.QRACKNET_SECRET_KEY ? process.env.QRACKNET_SECRET_KEY : crypto.randomBytes(256).toString('base64'),
  // Token is valid for 60 minutes (unless refreshed).
  expiresIn: 3600,
  // Algorithm used for encrypting JWT
  algorithm: 'HS256'
}

config.supportEmail = {
  service: process.env.QRACKNET_SUPPORT_EMAIL_SERVICE,
  account: process.env.QRACKNET_SUPPORT_EMAIL_ACCOUNT,
  password: process.env.QRACKNET_SUPPORT_EMAIL_PASSWORD,
  address: process.env.QRACKNET_SUPPORT_EMAIL_ADDRESS
}

config.pgPool = {
  user: 'qracknet',
  host: 'localhost',
  database: 'qracknet',
  password: process.env.PGPASSWORD,
  port: 5432
}

config.pgConnectionString = 'postgres://' + config.pgPool.user + ':' + config.pgPool.password + '@' + config.pgPool.host + ':' + config.pgPool.port + '/' + config.pgPool.database

module.exports = config
