
// https://www.youtube.com/watch?v=kgE8v5M1Eoo
// https://github.com/CppCon/CppCon2024/blob/main/Presentations/The_Most_Important_Design_Guideline_is_Testability.pdf

#include <functional>
#include <iostream>
#include <ostream>
#include <utility>
#include <cassert>

template <typename TagT, auto ...>
struct Sudo;

template <typename TagT>
struct Sudo<TagT>
{
    inline static typename TagT::type var{};
};

template <typename TagT, typename TagT::type Member>
struct Sudo<TagT, Member>
{
    inline static const decltype(Member) var = Sudo<TagT>::var = Member;
};

template <typename TagT, typename ObjectT>
decltype(auto) sudo(ObjectT &&object)
{
    assert(Sudo<TagT>::var);
    return std::invoke(Sudo<TagT>::var, std::forward<ObjectT>(object));
}

template <typename ClassT, typename MemberT, typename = void>
struct SudoTag
{
    using type = MemberT ClassT::*;
};

// initialize templates
namespace foo::bar { class Blarg { int x = 43;};}
using Blarg_x = SudoTag<foo::bar::Blarg, int>;

template struct Sudo<Blarg_x, &foo::bar::Blarg::x>;


int main()
{
    auto blarg = foo::bar::Blarg{};
    std::cout << sudo<Blarg_x>(foo::bar::Blarg{}) << std::endl;
}

struct A
{
    int i;
};

template <typename MemberType, typename ClassType>
struct TagHandle
{
    using type = MemberType ClassType::*;
};

int jjj() {
    int A::* i_ptr = &A::i;
    return 0;
}





