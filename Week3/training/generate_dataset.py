import chess # imports the python chess library helps to create and manipulate chess board
import chess.engine # this is a module which communicates with the stockfish
import numpy as np # used to store data efficiently
import random # used to play diff/random legal moves to get diff board positions for evaluation
from tqdm import tqdm # shows the progress bar

# -----------------------------
# Start Stockfish
# -----------------------------
STOCKFISH_PATH = "stockfish/stockfish-windows-x86-64-avx2.exe" # stores the path of the stockfish executable
engine = chess.engine.SimpleEngine.popen_uci(STOCKFISH_PATH) #starts the stockfish now python can send commands to stockfish

# neural networks don't understand chess board they only understand nos so we need to convert chess board into nos
# -----------------------------
# Convert board -> 768 features
# -----------------------------
def board_to_features(board): #converts board to features or nos
    features = np.zeros(768, dtype=np.float32)# creates a numpy array with all elements 0
# why 768 as we have 12 piecetypes and 64 squares
    for square in chess.SQUARES:# loop over all the squares of the chess board
        piece = board.piece_at(square) # this is function to check if there is a piece on the chess board
# if the piece stores a none means there is no piece on that square
        if piece is None: #if no piece on square move to next
            continue

        piece_index = {# this is a dictionary
            chess.PAWN: 0,# mapping of pieces to specific nos
            chess.KNIGHT: 1,
            chess.BISHOP: 2,
            chess.ROOK: 3,
            chess.QUEEN: 4,
            chess.KING: 5
        }[piece.piece_type]
        # if the current piece is white queen this function piece.piece_type returns chess.queen
# white uses 0 to 5 and black uses 6-11
        if piece.color == chess.BLACK:
            piece_index += 6 # so black indexing starts from 6 instead of 0 as in case of white

        features[piece_index * 64 + square] = 1.0

    return features # returns the vector


# -----------------------------
# Dataset generation
# -----------------------------
X = []
Y = []  # this stores stockfish evaluations

NUM_POSITIONS = 50000 # generate 50k board positions/configurations

for _ in tqdm(range(NUM_POSITIONS)):

    board = chess.Board() # start with the initial board position

    # Play random legal moves
    for _ in range(random.randint(0, 30)):
        if board.is_game_over():
            break

        move = random.choice(list(board.legal_moves))
        board.push(move)

    # Ask Stockfish
    info = engine.analyse(
        board,
        chess.engine.Limit(depth=10)
    )

    score = info["score"].white().score()

    if score is None:
        continue

    X.append(board_to_features(board))
    Y.append(score)


engine.quit()

X = np.array(X, dtype=np.float32)
Y = np.array(Y, dtype=np.float32)

np.savez("dataset.npz", X=X, Y=Y)

print("Dataset saved.")
print(X.shape)
print(Y.shape)