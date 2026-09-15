#include <iostream>

#include "system/modal.h"

namespace impl {

namespace modal = civilarium::system::modal;

}  // namespace impl

int main(int argc, char* argv[])
{
    std::cout << "hello" << std::endl;

    impl::modal::ShowErrorMessage("title", "msg");
    impl::modal::ShowErrorMessage("title", "msg", "quote");

    return 0;
}
