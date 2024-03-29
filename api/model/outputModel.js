// outputModel.js

'use strict'

function outputModel (sequelize, DataTypes) {
  const Model = sequelize.define('output', {
    name: {
      type: DataTypes.TEXT,
      allowNull: false
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

module.exports = outputModel
