// jobStatusTypeModel.js

'use strict'

module.exports = function (sequelize, DataTypes) {
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
