type parse('raw_t, 't) = 'raw_t => 't;
type serialize('t, 'raw_t) = 't => 'raw_t;
type query = string;

type graphqlDefinition('t, 'raw_t) = (
  parse('raw_t, 't),
  query,
  serialize('t, 'raw_t),
);

type variablesArg('variables) =
  // Js.Json.t is the type of Raw.t_variables in graphql-ppx when there are no variables
  | NoVariables: variablesArg(Js.Json.t)
  | Variables('variables): variablesArg('variables);

module type Operation = {
  let query: string;

  module Raw: {
    type t;
    type t_variables;
  };
  type t;

  let parse: Raw.t => t;
  let serialize: t => Raw.t;
};
