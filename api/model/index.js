'use strict'

const Sequelize = require('sequelize')
const config = require('../config/config')

const jobModel = require('./jobModel')
const jobStatusTypeModel = require('./jobStatusTypeModel')
const outputModel = require('./outputModel')
const outputTypeModel = require('./outputTypeModel')
const userModel = require('./userModel')

let sequelize
if (config.use_env_variable) {
  sequelize = new Sequelize(process.env[config.use_env_variable], config)
} else {
  sequelize = new Sequelize(config.database, config.username, config.password, config)
}

const db = {
  job: jobModel(sequelize, Sequelize.DataTypes),
  jobStatusType: jobStatusTypeModel(sequelize, Sequelize.DataTypes),
  output: outputModel(sequelize, Sequelize.DataTypes),
  outputType: outputTypeModel(sequelize, Sequelize.DataTypes),
  user: userModel(sequelize, Sequelize.DataTypes)
}

Object.keys(db).forEach(modelName => {
  if (db[modelName].associate) {
    db[modelName].associate(db)
  }
})

db.sequelize = sequelize

module.exports = db
