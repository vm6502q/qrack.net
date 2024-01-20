'use strict'

import fs from 'fs'
import path from 'path'
import Sequelize from 'sequelize'
import config from '../config/config.js'

import jobModel from './jobModel.js'
import jobStatusTypeModel from './jobStatusTypeModel.js'
import outputModel from './outputModel.js'
import outputTypeModel from './outputTypeModel.js'
import userModel from './userModel.js'

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

export default db
