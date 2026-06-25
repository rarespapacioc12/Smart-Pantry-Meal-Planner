#pragma once
#include <functional>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "../core/ShoppingItem.hpp"
#include "../io/JsonValue.hpp"

// ============================================================================
// Tema 3 - DESIGN PATTERN: Factory (registry-based)
// ----------------------------------------------------------------------------
// Construieste obiectul concret corect din ierarhia ShoppingItem pe baza unei
// specificatii JSON (`type` + atribute). Tipurile sunt inregistrate intr-o
// harta de "creators" in constructor, deci adaugarea unui nou tip de produs
// nu cere un lant suplimentar de if/else imprastiat prin cod.
//
// Este sursa UNICA de creare a produselor: DomainJson deleaga aici incarcarea
// listei de cumparaturi, iar MealPlanner il foloseste pentru a genera automat
// produsele lipsa - fara duplicarea logicii de creare.
// ============================================================================
class ShoppingItemFactory {
public:
    using Creator = std::function<std::unique_ptr<ShoppingItem>(const JsonValue&)>;

    ShoppingItemFactory();  // inregistreaza creators pentru tipurile built-in

    // Creeaza pe baza campului "type" din specificatie.
    std::unique_ptr<ShoppingItem> createFromJson(const JsonValue& spec) const;

    // Creeaza un tip dat explicit, folosind specificatia pentru restul campurilor.
    std::unique_ptr<ShoppingItem> create(const std::string& type, const JsonValue& spec) const;

    std::vector<std::string> registeredTypes() const;

private:
    std::map<std::string, Creator> _creators;
};
