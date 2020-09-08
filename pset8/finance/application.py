import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""

    # Query database for the users portfolio of stocks and cash
    portfolio = db.execute("SELECT stock, shares FROM holdings WHERE user_id = :user_id ORDER BY stock ASC",
        user_id = session["user_id"])

    total_stocks_value = 0

    # For each stock in the portfolio look up the current price, calculate value and total value
    for stock in portfolio:

        stock_info= lookup (stock['stock'])
        price = stock_info['price']
        stock.update ({ 'price': price })

        valuation = price * stock['shares']
        stock.update ({ 'valuation': valuation})

        total_stocks_value += valuation

    cash_sel = db.execute("SELECT cash FROM users WHERE id = :user_id",
        user_id= session["user_id"])
    cash = cash_sel[0]['cash']

    # Calculates the total balance of the users portfolio
    total = cash + total_stocks_value

    # Renders the homepage with the user portfolio
    return render_template('index.html', portfolio = portfolio, cash = round(cash, 2), total_stocks_value = round(total_stocks_value, 2), total = round(total, 2))



@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Lookup the data for the user inputted stock symbol
        stock = lookup(request.form.get('symbol'))

        # If the stock symbol does not exist display apology message
        if not stock:
            return apology("Not a valid stock", 403)

        # Retrieve the number of shares desired
        shares = int(request.form.get('shares'))

        # If the number of shares in a non positive integer, display apology
        if shares <= 0:
            return apology("Not a valid number of stocks", 403)

        # Calculates the total cost of the shares
        valuation =  stock['price'] * float(shares)

        # Selects the users cash
        cash = db.execute("SELECT cash FROM users WHERE id = :user",
            user = session["user_id"])[0]['cash']

        # Calculates the users remaining cash, and if user has insufficient funds returns an apology
        rem_cash = cash - valuation
        if rem_cash < 0:
            return apology("Insufficient funds", 403)

        # Otherwise removes the funds from the user's cash
        db.execute("UPDATE users SET cash = :cash WHERE id = :user",
            cash = rem_cash, user = session['user_id'])

        # Check if user already owns shares of the same stock
        holding = db.execute("SELECT shares FROM holdings WHERE user_id = :user AND stock = :stock",
            user = session["user_id"], stock = stock['symbol'])

        # If not, adds a new row in holdings table for the shares
        if not holding:
            db.execute("INSERT INTO holdings(user_id, stock, shares) VALUES (:user, :stock, :shares)",
                user = session["user_id"], stock = stock['symbol'], shares = shares)

        # Else updates the holdings table to include new shares
        else:
            total_shares = holding[0]['shares'] + shares
            db.execute("UPDATE holdings SET shares = :shares WHERE user_id = :user AND stock = :stock",
                user = session["user_id"], stock = stock['symbol'], shares = total_shares)

        # Logs the purchase into the history
        db.execute("INSERT INTO history(user_id, stock, shares, price, purchased_sold) VALUES (:user, :stock, :shares, :price, :purchased_sold)",
            user = session["user_id"], stock = stock['symbol'], shares = shares, price = stock['price'], purchased_sold = "Purchased")

        # Flashes a message of completion and redirects to home page
        if shares == 1:
            flash(f"Successfully purchased {shares} share of {stock['name']}")
        else:
            flash(f"Successfully purchased {shares} shares of {stock['name']}")
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("buy.html")

@app.route("/topup", methods=["GET", "POST"])
@login_required
def topup():
    """Buy shares of stock"""
    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Retrieves the cash to be topped up
        cash_add = int(request.form.get('cash'))

        # Selects the users cash balance
        cash = db.execute("SELECT cash FROM users WHERE id = :user",
            user = session["user_id"])[0]['cash']

        # Calculates the post topup cash balance and updates the users table
        cash = cash + cash_add

        db.execute("UPDATE users SET cash = :cash WHERE id = :user",
            user = session["user_id"], cash = cash)

        # Flashes a message of completion and redirects to home page
        flash(f"Successfully added funds")
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("topup.html")

@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    # Selects the history of transactions of the user
    history = db.execute("SELECT stock, shares, price, date_time, purchased_sold FROM history WHERE user_id = :user ORDER BY date_time DESC",
        user= session['user_id'])

    return render_template("history.html", history = history)

@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Lookup the data for the user inputted stock symbol
        stock = lookup(request.form.get("symbol"))

        # If the stock symbol does not exist display apology message
        if not stock:
            return apology("Not a valid stock", 403)

        # Display price information of the stock
        return render_template("quoted.html", stock = stock)

   # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("quote.html")

@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username to see if it already exists
        elif db.execute("SELECT * FROM users WHERE username = :username",
            username=request.form.get("username")):
            return apology("Username already exists", 403)

        # Check passwords match
        elif request.form.get("password") != request.form.get("confirm_password"):
            return apology("Password confirmation does not match", 403)

        # Add username and hash password to database
        db.execute("INSERT INTO users (username, hash) VALUES (:username, :hash)",
            username=request.form.get("username"), hash=generate_password_hash(request.form.get("password")))

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
            username=request.form.get("username"))

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Lookup the data for the user inputted stock symbol
        stock = lookup(request.form.get('symbol'))

        # If the stock symbol does not exist display apology message
        if not stock:
            return apology("Not a valid stock", 403)

        # Retrieve the number of shares desired
        shares = int(request.form.get('shares'))

        # If the number of shares in a non positive integer, display apology
        if shares <= 0:
            return apology("Not a valid number of stocks", 403)

        # Calculates the total cost of the shares
        valuation = stock['price'] * float(shares)

        # Query database for the users portfolio of stocks and cash
        shares_holding = db.execute("SELECT shares FROM holdings WHERE user_id = :user AND stock = :stock",
        user = session["user_id"], stock = stock['symbol'])[0]['shares']

        # Calculates the remaining shares the user owns after selling
        shares_rem = shares_holding - shares

        # If not enough shares, displays apology
        if shares_rem < 0:
            return apology("Unable to sell more shares than owned", 403)

        # Deletes the shares from the holdings table if all are sold
        elif shares_rem == 0:
            db.execute("DELETE FROM holdings WHERE user_id = :user AND stock = :stock",
                user = session["user_id"], stock = stock['symbol'])

        # Else updates the holdings table to account for sold shares
        else:
            db.execute("UPDATE holdings SET shares = :shares WHERE user_id = :user AND stock = :stock",
                user = session["user_id"], stock = stock['symbol'], shares = shares_rem)

        # Selects the users cash balance
        cash = db.execute("SELECT cash FROM users WHERE id = :user",
            user = session["user_id"])[0]['cash']

        # Calculates the post sale cash balance and updates the users table
        cash_postsale = cash + valuation

        db.execute("UPDATE users SET cash = :cash WHERE id = :user",
            user = session["user_id"], cash = cash_postsale)

        # Logs the purchase into the history
        db.execute("INSERT INTO history(user_id, stock, shares, price, purchased_sold) VALUES (:user, :stock, :shares, :price, :purchased_sold)",
            user = session["user_id"], stock = stock['symbol'], shares = shares, price = stock['price'], purchased_sold = "Sold")

        # Flashes a message of completion and redirects to home page
        if shares == 1:
            flash(f"Successfully sold {shares} share of {stock['name']}")
        else:
            flash(f"Successfully sold {shares} shares of {stock['name']}")
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        # Selects users portfolio to pass options for stocks to share
        portfolio = db.execute("SELECT stock FROM holdings WHERE user_id = :user ORDER BY stock ASC",
            user = session["user_id"])

        # Renders the sell page
        return render_template("sell.html", portfolio = portfolio)

def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)

