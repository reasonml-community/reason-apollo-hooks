module Graphql_Location = Apollo_Client__Graphql_Location;

type t = {
  message: string,
  locations: option(array(Graphql_Location.sourceLocation)),
  // Union? https://bucklescript.github.io/blog/2020/02/07/union-types-in-bucklescript
  path: option(array(string)) // ACTUAL: string | number
};
