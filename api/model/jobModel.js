// jobModel.js

'use strict'

function jobModel (sequelize, DataTypes) {
  const Model = sequelize.define('job', {
    statusMessage: {
      type: DataTypes.TEXT,
      allowNull: false
    },
  }, {})
  Model.associate = function (db) {
    db.user.hasMany(db.job)
    db.job.belongsTo(db.user)
  }
  return Model
}

module.exports = jobModel
