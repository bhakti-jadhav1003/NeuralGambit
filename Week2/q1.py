import json
import copy  # use it for deepcopy if needed
import math  # for math.inf
import logging

logging.basicConfig(format='%(levelname)s - %(asctime)s - %(message)s', datefmt='%d-%b-%y %H:%M:%S',
                    level=logging.INFO)

# Global variables in which you need to store player strategies (this is data structure that'll be used for evaluation)
# Mapping from histories (str) to probability distribution over actions
strategy_dict_x = {}
strategy_dict_o = {}


class History:
    def __init__(self, history=None):
        """
        # self.history : Eg: [0, 4, 2, 5]
            keeps track of sequence of actions played since the beginning of the game.
            Each action is an integer between 0-8 representing the square in which the move will be played as shown
            below.
              ___ ___ ____
             |_0_|_1_|_2_|
             |_3_|_4_|_5_|
             |_6_|_7_|_8_|

        # self.board
            empty squares are represented using '0' and occupied squares are either 'x' or 'o'.
            Eg: ['x', '0', 'x', '0', 'o', 'o', '0', '0', '0']
            for board
              ___ ___ ____
             |_x_|___|_x_|
             |___|_o_|_o_|
             |___|___|___|

        # self.player: 'x' or 'o'
            Player whose turn it is at the current history/board

        :param history: list keeps track of sequence of actions played since the beginning of the game.
        """
        if history is not None:
            self.history = history
            self.board = self.get_board()
        else:
            self.history = []
            self.board = ['0', '0', '0', '0', '0', '0', '0', '0', '0']
        self.player = self.current_player()

    def current_player(self):
        """ Player function
        Get player whose turn it is at the current history/board
        :return: 'x' or 'o' or None
        """
        total_num_moves = len(self.history)
        if total_num_moves < 9:
            if total_num_moves % 2 == 0:
                return 'x'
            else:
                return 'o'
        else:
            return None

    def get_board(self):
        """ Play out the current self.history and get the board corresponding to the history in self.board.

        :return: list Eg: ['x', '0', 'x', '0', 'o', 'o', '0', '0', '0']
        """
        board = ['0', '0', '0', '0', '0', '0', '0', '0', '0']
        for i in range(len(self.history)):
            if i % 2 == 0:
                board[self.history[i]] = 'x'
            else:
                board[self.history[i]] = 'o'
        return board

    def is_win(self):
        # check if the board position is a win for either players
        # Feel free to implement this in anyway if needed
        #there are 8 winning patterns depending on who gets it that player wins
        winning_patterns=[[0,1,2],
                          [3,4,5],
                          [6,7,8],
                          [0,3,6],
                          [1,4,7],
                          [2,5,8],
                          [0,4,8],
                          [2,4,6]
        ]
        
        for patterns in winning_patterns:
            a=patterns[0]
            b=patterns[1]
            c=patterns[2]
            if self.board[a]==self.board[b]==self.board[c]=='x':
                return 1
            if self.board[a]==self.board[b]==self.board[c]=='o':
                return -1
        return 0#this simply means there is no winning pattern but it doesnt mean a draw beacuse the board may be still partially filled and have chances of winning

    def is_draw(self):
        # check if the board position is a draw
        # Feel free to implement this in anyway if needed
        if self.is_win()==0:
            for i in range(9):
                if self.board[i]!='0':
                    continue
                else:
                    return False#the board is not full;game is still going
            return True#means the board has no winning pattern and no further moves are possible
        else:
            return False#if the board already has a win then no draw is possible

        

    def get_valid_actions(self):
        # get the empty squares from the board
        # Feel free to implement this in anyway if needed
        possible_actions=[]#create a empty list
        for i in range(9):#i iterates from i=0 to i=8
            if self.board[i]=='0':
                possible_actions.append(i)
            else:
                continue
        return possible_actions#gives all the empty boxes available for play


            

        

    def is_terminal_history(self):
        ##history==sequence of moves played so far
        ##terminal history is the complete sequence of moves from the start till the game terminates
        # check if the history is a terminal history
        # Feel free to implement this in anyway if needed
        if self.is_draw() or self.is_win():#if the game is a win or draw it means the end of game
            return True#since the game has ended history is a terminal history
        return False
        

    def get_utility_given_terminal_history(self):
        # Feel free to implement this in anyway if needed
        #it is known that the game has ended we just have to find the utility
        #if i write self.is_win repeatedly in every condn then the function is called many a times efficint store the result in a var
        result=self.is_win()
        if result==1:
            return 1
        elif result==-1:#since it is a terminal state it is a win or draw
            return -1
        else:
            return 0

    
            
        

    def update_history(self, action):
        # In case you need to create a deepcopy and update the history obj to get the next history object.
        # Feel free to implement this in anyway if needed
        new_history=self.history.copy()#make a copy of history
        new_history.append(action)#make changes in the newhistory list
        return History(new_history)#this creates a new object of the class by calling the constructor function

        #a node is a geme state 
        #history is a list and History is a object of the class whose elemnts are history,board,player     
#History.new_history is not defined as it is not an element of the class History

def backward_induction(history_obj):
    """
    :param history_obj: Histroy class object
    :return: best achievable utility (float) for th current history_obj
    """
    global strategy_dict_x, strategy_dict_o
    # TODO implement
    # (1) Implement backward induction for tictactoe
    # (2) Update the global variables strategy_dict_x or strategy_dict_o which are a mapping from histories to
    # probability distribution over actions.
    # (2a)These are dictionary with keys as string representation of the history list e.g. if the history list of the
    # history_obj is [0, 4, 2, 5], then the key is "0425". Each value is in turn a dictionary with keys as actions 0-8
    # (str "0", "1", ..., "8") and each value of this dictionary is a float (representing the probability of
    # choosing that action). Example: {”0452”: {”0”: 0, ”1”: 0, ”2”: 0, ”3”: 0, ”4”: 0, ”5”: 0, ”6”: 1, ”7”: 0, ”8”:
    # 0}}
    # (2b) Note, the strategy for each history in strategy_dict_x and strategy_dict_o is probability distribution over
    # actions. But since tictactoe is a PIEFG, there always exists an optimal deterministic strategy (SPNE). So your
    # policy will be something like this {"0": 1, "1": 0, "2": 0, "3": 0, "4": 0, "5": 0, "6": 0, "7": 0, "8": 0} where
    # "0" was the one of the best actions for the current player/history.
    if history_obj.is_terminal_history():#means have we reached the leaf node(base case)
        return history_obj.get_utility_given_terminal_history()
    else:
        actions=history_obj.get_valid_actions()#for the current board position get all the possible moves from this position
        best_move=[]#it stores all the possible moves to reach the best utility 
        if history_obj.player=='x':
            best_utility=-math.inf#in worst case the maximizing player can have a utility of - infinity
        else:
            best_utility=math.inf
        for action in actions:
            new_history=history_obj.update_history(action)#update the board after making the move
            child_utility=backward_induction(new_history)
#each action is a move at a depth and is evaluated till the leaf node from the depth
            if history_obj.player=='x':
                if child_utility>best_utility:
                    best_utility=child_utility
                    best_move=[action]
                elif child_utility==best_utility:
                    best_move.append(action)
            
                else:
                    continue

            else:
                if child_utility<best_utility:
                    best_utility=child_utility
                    best_move=[action]
                elif child_utility==best_utility:
                    best_move.append(action)
            
                else:
                    continue



        prob_dict={}
        for i in range(9):#from index 0 to 8
            if i in best_move:
                prob_dict[str(i)]=1/len(best_move)#str(i) is the key for the dict
            else:
                prob_dict[str(i)]=0#it stores the probability for the positions given the moves played so far
        
        key = "".join(str(a) for a in history_obj.history)#stores the moves played so far


        if history_obj.player=='x':
            strategy_dict_x[key]=prob_dict
        else:
            strategy_dict_o[key]=prob_dict




    return best_utility
                




    





    


def solve_tictactoe():
    backward_induction(History())
    with open('./policy_x.json', 'w') as f:
        json.dump(strategy_dict_x, f)
    with open('./policy_o.json', 'w') as f:
        json.dump(strategy_dict_o, f)
    return strategy_dict_x, strategy_dict_o


if __name__ == "__main__":
    logging.info("Start")
    solve_tictactoe()
    logging.info("End")