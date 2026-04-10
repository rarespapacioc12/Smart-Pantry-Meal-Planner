#include "../../include/exceptions/ItemNotFoundError.hpp"

ItemNotFoundError::ItemNotFoundError(const std::string& item_name)
: AppError("Item not found: " + item_name, "ITEM_NOT_FOUND"),
_item_name(item_name) {}

const std::string& ItemNotFoundError::item_name() const{
    return _item_name;
}
