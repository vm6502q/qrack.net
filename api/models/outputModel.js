// outputModel.js

'use strict'

module.exports = function (sequelize, DataTypes) {
  const Model = sequelize.define('output', {
    jobId: {
      type: DataTypes.INTEGER,
      allowNull: false
    },
    typeId: {
      type: DataTypes.INTEGER,
      allowNull: false
    },
    name: {
      type: DataTypes.TEXT,
      allowNull: false,
    },
    value: {
      type: DataTypes.TEXT,
      allowNull: false
    }
  }, {})
  Model.associate = function (db) {
    db.job.hasMany(db.output)
    db.output.belongsTo(db.job)
  }
  return Model
}
