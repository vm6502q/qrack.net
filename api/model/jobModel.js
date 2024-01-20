// jobModel.js

'use strict'

function jobModel (sequelize, DataTypes) {
  const Model = sequelize.define('job', {}, {})
  Model.associate = function (db) {
    db.user.hasMany(db.job)
    db.job.belongsTo(db.user)
  }
  return Model
}

export default jobModel
