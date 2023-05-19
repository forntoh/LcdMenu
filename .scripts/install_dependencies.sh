# Example build script
bundle install

# ensure the Arduino installation -- creates the Library directory
bundle exec ensure_arduino_installation.rb

# manually install a custom library from a git repository
git clone https://github.com/johnrickman/LiquidCrystal_I2C.git
mv LiquidCrystal_I2C $(bundle exec arduino_library_location.rb)

bundle exec arduino_ci.rb --skip-examples-compilation