module Source = ApolloClient__Graphql_Language_Source;

// export interface FieldNode {
//   readonly kind: 'Field';
//   readonly loc?: Location;
//   readonly alias?: NameNode;
//   readonly name: NameNode;
//   readonly arguments?: ReadonlyArray<ArgumentNode>;
//   readonly directives?: ReadonlyArray<DirectiveNode>;
//   readonly selectionSet?: SelectionSetNode;
// }

module TokenKindEnum = {
  [@bs.deriving jsConverter]
  type t = [
    | `SOF
    | `EOF
    | `BANG
    | `DOLLAR
    | `AMP
    | `PAREN_L
    | `PAREN_R
    | `SPREAD
    | `COLON
    | `EQUALS
    | `AT
    | `BRACKET_L
    | `BRACKET_R
    | `BRACE_L
    | `PIPE
    | `BRACE_R
    | `NAME
    | `INT
    | `FLOAT
    | `STRING
    | `BLOCK_STRING
    | `COMMENT
  ];
};

module Token = {
  type t = {
    kind: TokenKindEnum.t,
    start: int,
    [@bs.as "end"]
    end_: int,
    line: int,
    column: int,
    value: option(string),
    prev: Js.nullable(t),
    next: Js.nullable(t),
  };
};

module Location = {
  type t = {
    start: int,
    [@bs.as "end"]
    end_: int,
    startToken: Token.t,
    endToken: Token.t,
    source: Source.t,
  };
};

module NameNode = {
  type t = {
    kind: string,
    loc: option(Location.t),
    value: string,
  };
};

module NamedTypeNode = {
  type t = {
    kind: string,
    loc: option(Location.t),
    name: NameNode.t,
  };
};

module ValueNode = {
  type t;
};

module ArgumentNode = {
  type t = {
    kind: string,
    loc: option(Location.t),
    name: NameNode.t,
    value: ValueNode.t,
  };
};

module DirectiveNode = {
  type t = {
    kind: string,
    loc: option(Location.t),
    name: NameNode.t,
    arguments: option(array(ArgumentNode.t)),
  };
};

module SelectionNode = {
  type t;
};

module SelectionSetNode = {
  type t = {
    kind: string,
    loc: option(Location.t),
    selections: array(SelectionNode.t),
  };
};

module FieldNode = {
  type t = {
    kind: string,
    loc: option(Location.t),
    alias: option(NameNode.t),
    name: NameNode.t,
    arguments: option(array(ArgumentNode.t)),
    directives: option(array(DirectiveNode.t)),
    selectionSet: option(SelectionSetNode.t),
  };
};

module FragmentDefinitionNode = {
  type t = {
    kind: string,
    loc: option(Location.t),
    name: NameNode.t,
    // // Note: fragment variable definitions are experimental and may be changed
    // // or removed in the future.
    // readonly variableDefinitions?: ReadonlyArray<VariableDefinitionNode>;
    typeCondition: NamedTypeNode.t,
    directives: option(array(DirectiveNode.t)),
    selectionSet: SelectionSetNode.t,
  };
};
