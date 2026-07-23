import torch
import torch.nn as nn
import torch.optim as optim
import numpy as np

data = np.load("dataset.npz")

X = torch.tensor(data["X"], dtype=torch.float32)
Y = torch.tensor(data["Y"], dtype=torch.float32).view(-1,1)

class NNUE(nn.Module):

    def __init__(self):
        super().__init__()

        self.network = nn.Sequential(
            nn.Linear(768,256),
            nn.ReLU(),
            nn.Linear(256,128),
            nn.ReLU(),
            nn.Linear(128,1)
        )

    def forward(self,x):
        return self.network(x)


model = NNUE()

criterion = nn.MSELoss()

optimizer = optim.Adam(model.parameters(), lr=0.001)

epochs = 40

for epoch in range(epochs):

    prediction = model(X)

    loss = criterion(prediction,Y)

    optimizer.zero_grad()

    loss.backward()

    optimizer.step()

    print(epoch+1, loss.item())

torch.save(model.state_dict(),"nnue_weights.pth")

print("Training Complete")