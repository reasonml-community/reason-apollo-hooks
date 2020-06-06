module FragmentDefinitionNode = ApolloClient__Graphql_Language_Ast.FragmentDefinitionNode;

module FragmentMap = {
  module Js_ = {
    type t = Js.Dict.t(FragmentDefinitionNode.t);
  };

  type t = Js_.t;
};
