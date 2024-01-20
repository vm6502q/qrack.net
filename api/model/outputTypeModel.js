// outputModel.js

'use strict'

function outputTypeModel (sequelize, DataTypes) {
  const Model = sequelize.define('outputType', {
    name: {
      type: DataTypes.TEXT,
      allowNull: false
    }
  }, {})
  Model.associate = function (db) {
    db.outputType.hasMany(db.output)
    db.output.belongsTo(db.outputType)
  }
  return Model
}

export default outputTypeModel
