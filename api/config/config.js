const config = require('../config')

config.database = config.pgPool.database
config.username = config.pgPool.user
config.password = config.pgPool.password
config.dialect = 'postgres'
config.host = config.pgPool.host
config.port = config.pgPool.port

module.exports = config
