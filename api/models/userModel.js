// userModel.js

'use strict'

const { v4: uuidv4 } = require('uuid')
const recoveryExpirationMinutes = 30
const millisPerMinute = 60000

module.exports = function (sequelize, DataTypes) {
  const Model = sequelize.define('user', {
    username: {
      type: DataTypes.TEXT,
      allowNull: false
    },
    usernameNormal: {
      type: DataTypes.TEXT,
      allowNull: false
    },
    passwordHash: {
      type: DataTypes.TEXT,
      allowNull: false
    },
    affiliation: {
      type: DataTypes.TEXT,
      defaultValue: '',
      allowNull: false
    },
    name: {
      type: DataTypes.TEXT,
      defaultValue: '',
      allowNull: false
    },
    email: {
      type: DataTypes.TEXT,
      allowNull: false
    },
    recoveryToken: {
      type: DataTypes.TEXT
    },
    recoveryTokenExpiration: {
      type: DataTypes.DATE
    }
  }, {})
  Model.generateRecovery = function () {
    this.recoveryToken = uuidv4()
    this.recoveryTokenExpiration = new Date((new Date()).getTime() + recoveryExpirationMinutes * millisPerMinute)
  }
  return Model
}
