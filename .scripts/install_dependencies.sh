# Example build script
bundle install

# ensure the Arduino installation -- creates the Library directory
bundle exec ensure_arduino_installation.rb

# manually install a custom library from a git repository
git clone https://github.com/johnrickman/LiquidCrystal_I2C.git
mv LiquidCrystal_I2C $(bundle exec arduino_library_location.rb)

# pio lib --storage-dir $(bundle exec arduino_library_location.rb) install marcoschwartz/LiquidCrystal_I2C@^1.1.4