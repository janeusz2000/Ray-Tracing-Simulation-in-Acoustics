const path = require('path');
module.exports = {
  mode : 'development',
  devtool : false,
  entry : './gui/index.js',
  output : {filename : 'main.js', path : path.resolve(__dirname, 'dist')},
};
