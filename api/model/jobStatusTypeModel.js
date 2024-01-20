// jobStatusTypeModel.js

'use strict'

function jobStatusTypeModel (sequelize, DataTypes) {
  const Model = sequelize.define('jobStatusType', {
    name: {
      type: DataTypes.TEXT,
      allowNull: false
    }
  }, {})
  Model.associate = function (db) {
    db.jobStatusType.hasMany(db.job)
    db.job.belongsTo(db.jobStatusType)
  }
  return Model
}

module.exports = jobStatusTypeModel
