import config from '../config.js'

config.database = config.pgPool.database
config.username = config.pgPool.user
config.password = config.pgPool.password
config.dialect = 'postgres'
config.host = config.pgPool.host
config.port = config.pgPool.port

export default config
