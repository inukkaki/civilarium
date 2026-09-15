#ifndef CIVILARIUM_SYSTEM_MODAL_H_
#define CIVILARIUM_SYSTEM_MODAL_H_

namespace civilarium::system::modal {

void ShowErrorMessage(const char title[], const char message[]);
void ShowErrorMessage(
    const char title[], const char message[], const char quotation[]);

}  // namespace civilarium::system::modal

#endif  // CIVILARIUM_SYSTEM_MODAL_H_
