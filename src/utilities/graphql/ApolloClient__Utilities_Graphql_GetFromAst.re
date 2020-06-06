module Graphql = ApolloClient__Graphql;

[@bs.module "@apollo/client/utilities"]
external getOperationDefinition:
  Graphql.documentNode =>
  option(Graphql.Language.Ast.OperationDefinitionNode.t) =
  "getOperationDefinition";
