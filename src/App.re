[%raw {|
  require("./App.css")
  |}];

type language =
  | EN
  | DE;

type game = {
  word: string,
  wordLanguage: language,
  solution: string,
  solutionLanguage: language,
};

type result =
  | CORRECT
  | WRONG;

let games = [
  {word: "Game", wordLanguage: EN, solution: "Spiel", solutionLanguage: DE},
  {word: "Apple", wordLanguage: EN, solution: "Apfel", solutionLanguage: DE},
  {
    word: "Entschuldigung",
    wordLanguage: DE,
    solution: "Sorry",
    solutionLanguage: EN,
  },
  {
    word: "Flowers",
    wordLanguage: EN,
    solution: "Blumen",
    solutionLanguage: DE,
  },
  {
    word: "Kortoffeln",
    wordLanguage: DE,
    solution: "Potatoes",
    solutionLanguage: EN,
  },
];

let generateGame = () => {
  let [game, ..._] = Belt.List.shuffle(games);
  game;
};
let languageToEmoji = language =>
  language === EN ? {js|🇺🇸|js} : {js|🇩🇪|js};

let resultToEmoji = result => {
  switch (result) {
  | Some(CORRECT) => {js|✅|js}

  | Some(WRONG) => {js|❌|js}

  | None => ""
  };
};

type state = {
  game,
  suggestion: string,
  result: option(result),
};

type action =
  | SUGGESTION_CHANGE(string)
  | FORM_SUBMIT
  | GAME_RESET;

[@react.component]
let make = () => {
  let initialState = {game: generateGame(), suggestion: "", result: None};

  let reducer = (state, action) => {
    switch (action) {
    | SUGGESTION_CHANGE(suggestion) => {...state, suggestion}
    | FORM_SUBMIT =>
      let result = state.suggestion === state.game.solution ? CORRECT : WRONG;
      {...state, result: Some(result)};

    | GAME_RESET => initialState
    };
  };

  let (state, dispatch) = React.useReducer(reducer, initialState);

  let handleInputChange = event => {
    let value = ReactEvent.Form.target(event)##value;
    dispatch(SUGGESTION_CHANGE(value));
  };

  let handleFormSubmit = event => {
    ReactEvent.Form.preventDefault(event);
    dispatch(FORM_SUBMIT);
  };

  <div className="App">
    <h1> {"Memorise" |> ReasonReact.string} </h1>
    <form onSubmit=handleFormSubmit>
      <div className="inputField word">
        <span className="flag">
          {state.game.wordLanguage |> languageToEmoji |> ReasonReact.string}
        </span>
        <input value={state.game.word} disabled=true />
      </div>
      <div className="inputField suggestion">
        <span className="flag">
          {state.game.solutionLanguage |> languageToEmoji |> ReasonReact.string}
        </span>
        <input
          placeholder="?"
          value={state.suggestion}
          onChange=handleInputChange
        />
      </div>
      <div className="buttonsRow">
        <button type_="submit"> {"ANSWER" |> ReasonReact.string} </button>
        <button onClick={_ => dispatch(GAME_RESET)} type_="button">
          {"RESET" |> ReasonReact.string}
        </button>
      </div>
      <div className="result">
        {state.result |> resultToEmoji |> ReasonReact.string}
      </div>
    </form>
  </div>;
};