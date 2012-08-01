import cairoplot as cp

#Question 1
q1 = { 'yes': [9, 14]}
cp.vertical_bar_plot('q1.svg', q1, 400,300, border=20, display_values=True, grid=True, rounded_corners=True, colors=["green", "red"],x_labels=["Q1: Did the activity take too long?","q2"])#, "red"])

