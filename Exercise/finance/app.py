import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""

    user_id = session["user_id"]

    user = db.execute("SELECT cash FROM users WHERE id = ?", user_id)
    cash = user[0]["cash"]

    rows = db.execute("""
        SELECT symbol, SUM(shares) AS total_shares FROM transactions
        WHERE user_id = ? GROUP BY symbol HAVING total_shares > 0""",user_id)
    portfolio = []
    total_value = cash

    for row in rows:
        quote = lookup(row["symbol"])
        if quote is None:
            continue

        stock_value = row["total_shares"] * quote["price"]
        total_value += stock_value

        portfolio.append({
            "symbol": row["symbol"],
            "shares": row["total_shares"],
            "price": quote["price"],
            "total": stock_value,
            "name": quote["name"]  
            })

    return render_template("index.html", cash=cash, total=total_value, portfolio=portfolio)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "GET":
        return render_template("buy.html")

    symbol = request.form.get("symbol")
    shares = request.form.get("shares")

    if not symbol:
        return apology("must provide symbol")

    quote = lookup(symbol)
    if quote is None:
        return apology("invalid symbol")

    if not shares or not shares.isdigit():
        return apology("shares must be positive integer")

    shares = int(shares)
    if shares <= 0:
        return apology("shares must be positive integer")

    price = quote["price"]
    cost = price * shares
    user_id = session["user_id"]

    cash = db.execute("SELECT cash FROM users WHERE id = ?", user_id)[0]["cash"]

    if cost > cash:
        return apology("cannot afford")

    db.execute("UPDATE users SET cash = cash - ? WHERE id = ?", cost, user_id)

    db.execute(
        "INSERT INTO transactions (user_id, symbol, shares, price) VALUES (?, ?, ?, ?)",
        user_id,
        quote["symbol"],
        shares,
        price
    )

    return redirect("/")



@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    rows = db.execute("SELECT symbol, shares, price, timestamp FROM history WHERE user_id = ?", session["user_id"])


    return render_template("history.html", rows=rows)


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
        rows = db.execute(
            "SELECT * FROM users WHERE username = ?", request.form.get("username")
        )

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(
            rows[0]["hash"], request.form.get("password")
        ):
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
    if request.method == "GET":
        return render_template("quote.html")

    symbol = request.form.get("symbol")

    if not symbol:
        return apology("Must provide symbol", 400)

    quote = lookup(symbol)

    if quote is None:
        return apology("Invalid symbol", 400)

    return render_template("quote.html", quote=quote)


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    if request.method == "GET":
        return render_template("register.html")
    username = request.form.get("username")
    password = request.form.get("password")
    confirmation = request.form.get("confirmation")


    if not username or not password or not confirmation:
        return apology("Must fill all the blank fields", 400)
    if password != confirmation:
        return apology("Passwords do not match", 400)
    
    rows = db.execute("SELECT * FROM users WHERE username = ?", username)
    if len(rows) != 0:
        return apology("username already taken", 400)
    
    hash_pw = generate_password_hash(password)
    new_id = db.execute ("INSERT INTO users (username, hash) VALUES (?,?)", username, hash_pw,)

    session["user_id"] = new_id
    return redirect("/")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    user_id = session["user_id"]

    if request.method == "GET":
        symbols = db.execute(
            "SELECT symbol FROM transactions WHERE user_id = ? GROUP BY symbol HAVING SUM(shares) > 0",
            user_id
        )
        return render_template("sell.html", symbols=symbols)

    symbol = request.form.get("symbol")
    shares = request.form.get("shares")

    if not symbol:
        return apology("must provide symbol")

    if not shares or not shares.isdigit():
        return apology("shares must be positive integer")

    shares = int(shares)
    if shares <= 0:
        return apology("shares must be positive integer")

    rows = db.execute(
        "SELECT SUM(shares) AS total FROM transactions WHERE user_id = ? AND symbol = ?",
        user_id,
        symbol
    )
    owned = rows[0]["total"]

    if owned is None or owned < shares:
        return apology("not enough shares")

    quote = lookup(symbol)
    price = quote["price"]

    db.execute("UPDATE users SET cash = cash + ? WHERE id = ?", price * shares, user_id)

    db.execute(
        "INSERT INTO transactions (user_id, symbol, shares, price) VALUES (?, ?, ?, ?)",
        user_id,
        symbol,
        -shares,
        price
    )

    return redirect("/")

@app.route("/addcash", methods=["GET", "POST"])
@login_required
def addcash():
    if request.method == "POST":
        amount = request.form.get("amount")
        try:
            amount = float(amount)
        except:
            return apology("invalid amount")

        if amount <= 0:
            return apology("must be positive")

        db.execute("UPDATE users SET cash = cash + ? WHERE id = ?", amount, session["user_id"])
        return redirect("/")

    return render_template("addcash.html")
