#include "MenuItem.h"

bool MenuItem::_isEditing = false;

bool MenuItem::isEditing() { return _isEditing; }
void MenuItem::beginEdit() { _isEditing = true; }
void MenuItem::endEdit() { _isEditing = false; }
