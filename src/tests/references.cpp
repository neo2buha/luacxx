#include "init.hpp"

#include "LuaGlobalAccessible.hpp"
#include "LuaProxyAccessible.hpp"

BOOST_AUTO_TEST_CASE(testLuaHandlesReferencesProperly)
{
    LuaEnvironment lua;
    LuaStack stack(lua);
    stack << "No Time";
    LuaReference ref(lua.luaState(), LuaReferenceAccessible(lua.luaState(), stack.save()));
    BOOST_CHECK_EQUAL(stack.size(), 1);
    lua::push(stack, ref);
    BOOST_CHECK_EQUAL(stack.get<const char*>(), "No Time");
}

BOOST_AUTO_TEST_CASE(testAccessibleCanGetAReference)
{
    LuaEnvironment lua;
    LuaReferenceAccessible accessor(lua.luaState());
    LuaStack s(lua);
    s << "No Time";
    accessor.store(s);
    BOOST_REQUIRE_EQUAL(s.get<const char*>(), "No Time");
    s.clear();
    accessor.push(s);
    BOOST_REQUIRE_EQUAL(s.get<const char*>(), "No Time");
}

BOOST_AUTO_TEST_CASE(testAccessibleCanGetAGlobal)
{
    LuaEnvironment lua;
    LuaGlobalAccessible accessor("foo");
    LuaStack s(lua);
    s << "No Time";
    accessor.store(s);
    BOOST_REQUIRE_EQUAL((const char*)lua["foo"], "No Time");
    s.clear();
    accessor.push(s);
    BOOST_REQUIRE_EQUAL(s.get<const char*>(), "No Time");
}

BOOST_AUTO_TEST_CASE(testLuaValueGetsALength)
{
    LuaEnvironment lua;
    lua("foo = {42, 42, 42}");
    BOOST_REQUIRE_EQUAL(lua["foo"].length(), 3);
}

BOOST_AUTO_TEST_CASE(testLuaProxy)
{
    LuaEnvironment lua;

    LuaGlobal foo = lua["foo"];
    lua["foo"] = 42;
    LuaProxy fooCopy = foo;

    BOOST_CHECK_EQUAL(42, fooCopy.get<int>());
}