import torch
import numpy as np

# Same architecture used during training
class NNUE(torch.nn.Module):
    def __init__(self):
        super().__init__()
        self.network = torch.nn.Sequential(
            torch.nn.Linear(768, 256),
            torch.nn.ReLU(),
            torch.nn.Linear(256, 128),
            torch.nn.ReLU(),
            torch.nn.Linear(128, 1)
        )

    def forward(self, x):
        return self.network(x)

# Load trained model
model = NNUE()
model.load_state_dict(torch.load("nnue_weights.pth"))
model.eval()

# Export every parameter into one binary file
weights = []

for param in model.parameters():
    weights.extend(param.detach().numpy().flatten())

weights = np.array(weights, dtype=np.float32)
weights.tofile("nnue_weights.bin")

print("Exported", len(weights), "weights")