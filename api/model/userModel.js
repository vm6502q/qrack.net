// userModel.js

'use strict'

import { v4 } from 'uuid'
const recoveryExpirationMinutes = 30
const millisPerMinute = 60000

function userModel (sequelize, DataTypes) {
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
    this.recoveryToken = v4.uuidv4()
    this.recoveryTokenExpiration = new Date((new Date()).getTime() + recoveryExpirationMinutes * millisPerMinute)
  }
  return Model
}

export default userModel
