# Smart Pantry & Meal Planner (C++ / OOP + GUI)

Aplicație desktop care gestionează inventarul (cămara/frigiderul), rețetele, planificarea meselor și lista de cumpărături. Aplicația încarcă datele din fișiere (CSV/JSON), permite editarea lor din interfață, rulează recomandări și generează automat un plan de mese și lista de cumpărături, apoi salvează modificările înapoi în fișiere.

Proiectul este structurat pe module (core / services / storage / gui), astfel încât logica să fie separată de interfața grafică.

---

## Funcționalități

### Inventar
- Vizualizare inventar ca tabel: produs, cantitate, unitate, locație (Pantry/Fridge/Freezer), dată expirare.
- Adăugare / editare / ștergere produse.
- Filtrare și sortare:
  - produse care expiră în următoarele N zile
  - căutare după nume / locație
- Consum automat:
  - consum manual (scădere cantitate)
  - consum pe rețetă (când “gătești”)

### Rețete
- Listă de rețete cu detalii:
  - ingrediente + cantități
  - pași
  - tag-uri (ex: vegetarian, quick, breakfast)
  - timp de preparare
- Recomandări: afișează rețetele cele mai potrivite în funcție de inventarul curent (cele cu cele mai puține ingrediente lipsă).
- Scalare rețetă după porții.

### Plan de mese (Meal Plan)
- Generare automată plan pe N zile (ex: 7) folosind rețete din RecipeBook.
- Editare manuală plan: schimbare rețetă/porții pentru o zi și un tip de masă.
- Salvare/încărcare plan în JSON.

### Listă de cumpărături + cost
- Generare listă de cumpărături pe baza planului:
  - calculează ingredientele lipsă și cantitățile necesare
  - agregă duplicatele
- Estimare cost total folosind un catalog de prețuri (CSV).
- Export listă de cumpărături în CSV.

---

## Cum funcționează (flux de utilizare)

1. La pornire, aplicația citește fișierele din `data/`:
   - inventarul (CSV)
   - rețetele (JSON)
   - prețurile (CSV)
   - planul (JSON, dacă există)
2. Utilizatorul editează inventarul sau rețetele din GUI.
3. Utilizatorul poate:
   - vedea rețete recomandate pe baza inventarului
   - genera un plan de mese pe N zile
4. Pe baza planului, aplicația generează lista de cumpărături și poate estima costul.
5. La salvare/ieșire, aplicația scrie înapoi datele actualizate în fișiere.

---

## Structura proiectului

```text
.
├─ CMakeLists.txt
├─ README.md
├─ data/
│  ├─ inventory.csv
│  ├─ recipes.json
│  ├─ prices.csv
│  └─ mealplan.json
├─ include/
│  ├─ core/         # model domeniu (Ingredient, Inventory, Recipe, MealPlan...)
│  ├─ services/     # logică (PlannerEngine, Recommender, Validators)
│  ├─ storage/      # persistență (DataSource + derivate)
│  ├─ gui/          # UI (Qt/ImGui) + controller
│  ├─ exceptions/   # ierarhia de excepții
│  └─ utils/        # Date, parsing, helpers
└─ src/
   ├─ main.cpp
   ├─ core/
   ├─ services/
   ├─ storage/
   ├─ gui/
   ├─ exceptions/
   └─ utils/
