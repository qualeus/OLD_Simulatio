#include "../../include/Constraints/Constraint.hpp"
namespace phy {
Constraint::Constraint() {}

int Constraint::get_id() const { return this->id; }
int Constraint::get_class() const { return ID_CONSTRAINT; }
static int id_class() { return ID_CONSTRAINT; }

bool Constraint::Equals(const Constraint* other) { return true; }
inline bool Constraint::operator==(const Constraint* other) { return this->Equals(other); }

}  // namespace phy