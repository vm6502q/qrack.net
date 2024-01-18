// jobModel.js

'use strict'

module.exports = function (sequelize, DataTypes) {
  const Model = sequelize.define('job', {
    statusId: {
      type: DataTypes.Integer,
      allowNull: false
    },
    userId: {
      type: DataTypes.INTEGER,
      allowNull: false
    }
  }, {})
  Model.associate = function (db) {
    db.user.hasMany(db.job)
    db.job.belongsTo(db.user)
  }
  return Model
}
