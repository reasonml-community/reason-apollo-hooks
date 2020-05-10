module Graphql_Location = Apollo_Client__Graphql_Location;

type t = {
  message: string,
  locations: Js.nullable(array(Graphql_Location.sourceLocation)),
  // Union? https://bucklescript.github.io/blog/2020/02/07/union-types-in-bucklescript
  path: Js.nullable(array(string)) // ACTUAL: string | number
};
