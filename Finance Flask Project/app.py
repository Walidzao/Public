import os

from datetime import datetime
from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash
from datetime import datetime
from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


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
    # display a table showing which stocks are owned by the user
    # Use jinja for
    # Select queries to retrive number of shares owned per stock for the current logged user.
    # Lookup to give the current stock price for each element
    # Stocks owned by the user
    # Numbers of shares owned, the current price of each stock, and the total value of each holding
    # Current cash balance
    # Grand total (i.e., stocks’ total value plus cash)

    # Retrieve username as it is the name of the user table of shares bought
    #define user in the session:
    user_id= session["user_id"]

    #table name being the username as it is unique.
    user_tableName = db.execute("Select username FROM users where id = ?", user_id)

    cashQuery = db.execute("SELECT cash from users WHERE id = ?",user_id)
    remainingDeposit = cashQuery[0]["cash"]
    try:
        stockShares = db.execute("SELECT symbol, SUM(shares) as ShareSum FROM ? where type = 'buy' GROUP BY symbol ",user_tableName[0]["username"])
    except :
        return render_template("index.html", cash = remainingDeposit,totalHoldingsValue = 0 )

    soldShares = db.execute("SELECT symbol, SUM(shares) as ShareSum FROM ? where type = 'sell' GROUP BY symbol ",user_tableName[0]["username"])

    if not stockShares:
        return render_template("index.html", cash = remainingDeposit,totalHoldingsValue = 0)


    stockPrice = []
    holdingValue, totalHoldingsValue = 0,0
    for stock in stockShares :
        try:
            soldSharesNumber =  next(item for item in soldShares if item["symbol"] == stock["symbol"])
        except StopIteration:
            soldSharesNumber =None
        #return apology(soldSharesNumber["symbol"] + str(soldSharesNumber["ShareSum"]))
        if soldSharesNumber :
            sharesInPosession = stock["ShareSum"] - soldSharesNumber["ShareSum"]
            stock.update({"ShareSum": sharesInPosession})
            #return apology(str(stock["ShareSum"]))
        quote = lookup(stock["symbol"])
        stock.update({"price": quote["price"]})
        holdingValue = quote["price"] * stock["ShareSum"]
        if holdingValue > 0 :
            totalHoldingsValue += holdingValue

    return render_template("index.html", cash = remainingDeposit, stockShares = stockShares, stockPrice = stockPrice, totalHoldingsValue= totalHoldingsValue)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":
        quote= lookup(request.form.get("symbol"))
        if quote is None:
            return apology("The stock you are searching for is not valid")
        # define data about price
        price = quote["price"]
        symbol = quote["symbol"]

        shares = request.form.get("shares")
        if not shares :
            return apology("The number of shares to be purchased should be provided")
        try:
            shares= int(shares)
        except ValueError as v:
            return apology("you have provided an invalid entry for the number of shares")

        if shares < 1 :
            return apology("The number of shares to be purchased should be a positive number")

        else :

            #define user in the session:
            user_id= session["user_id"]

            #table name being the username as it is unique.
            user_tableName = db.execute("Select username FROM users where id = ?", user_id)
            available_balance= db.execute("SELECT cash from users WHERE id = ?",user_id)
            purchasePrice = shares * price
            if available_balance[0]["cash"] < purchasePrice :
                return apology("Top up your cash buddy")
            else:
                # Symbol TEXT NOT NULL, price NUMERIC NOT NULL, time text NOT NULL
                #if user_tableName[0]["username"]
                #Create a new table for the user to regiter all the stock purchases recorded.
                try:
                    db.execute("CREATE TABLE ? (id INTEGER , symbol TEXT , shares INTEGER, price REAL, type TEXT, time TEXT )", user_tableName[0]["username"])
                except RuntimeError as err:
                    if "already exists" in str(err):
                        pass
                    else:
                        return apology(f"{err=}, {type(err)=}")
                #record time of purchase
                now = datetime.now()
                current_time = now.strftime("%D %H:%M:%S")

                #update the remaining cash in hand
                db.execute("UPDATE users SET cash = ? WHERE id = ?", available_balance[0]["cash"] - purchasePrice, user_id)

                #record the purchase, log the stock, time of purchase, number of shares and the price of acquisition of the shares.

                db.execute("INSERT INTO ?(id, symbol, shares, price, type, time) VALUES(?,?,?,?,'buy',?)",user_tableName[0]["username"],user_id, symbol,shares, purchasePrice ,current_time)

                return redirect("/")
    else:
        return render_template("buy.html")

@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    user_id= session["user_id"]
    #table name being the username as it is unique.
    user_tableName = db.execute("Select username FROM users where id = ?", user_id)
    operations  = db.execute("SELECT * FROM ?", user_tableName[0]["username"])
    return render_template("history.html", operations = operations)

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
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

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

    if request.method == "POST" :
        symbol = request.form.get("symbol")
        if not symbol:
            return apology("You did not input any Stock symbol")
        else :
            quote = lookup(symbol)
            if quote is None :
                return apology("Please input another Stock, the symbol is not found")
            return render_template("quoted.html", quote = lookup(symbol))

    else :
        return render_template("quote.html")




@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "POST" :
        username = request.form.get("username")
        if not username :
            return apology("username missing")
        usernames = db.execute("SELECT username FROM users WHERE username = ?", username)
        if usernames :
            return apology("username already exists, choose another name")
        password =request.form.get("password")
        if not password :
            return apology("password missing")

        confirmation = request.form.get("confirmation")
        if confirmation != password:
            return apology("the passwords do not match")
        else:
            hashed= generate_password_hash(password)
            db.execute("INSERT INTO users (username, hash) VALUES(?,?)",username, hashed)
            return redirect("/")
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    if request.method == "POST" :
        # Render an apology if the user fails to select a stock or if (somehow, once submitted) the user does not own any shares of that stock.
        #validating user input
        quote = request.form.get("symbol")
        if not quote:
            return apology("no stock inputed")
        quote = lookup(quote)
        if quote is None:
            return apology("The stock you are searching for is not valid")
        symbol = quote["symbol"]
        shares = request.form.get("shares")
        if not shares:
            return apology("number of shares not inputed")
        try:
            shares = int(shares)
        except ValueError :
            return apology("number of shares must be a number")
        if shares < 1:
            return apology("the number of shares must be at least 1")
        # Getting userID
        user_id= session["user_id"]
        user_tableName = db.execute("Select username FROM users where id = ?", user_id)
        # Retrieveing the name of the buy/sell log table of the user.
        tableUser = user_tableName[0]["username"]

        #values = db.execute("SELECT * FROM ? Where symbol = ?", tableUser ,symbol )

        boughtShares = db.execute("SELECT symbol, SUM(shares) as ShareSum FROM ? where type = 'buy' and symbol = ? GROUP BY symbol ",user_tableName[0]["username"], symbol)
        soldShares = db.execute("SELECT symbol, SUM(shares) as ShareSum FROM ? where type = 'sell' and symbol = ? GROUP BY symbol ",user_tableName[0]["username"],symbol)
        if not boughtShares:
            return apology("you don't posess any shares of this company")
        elif not soldShares :
            sharesInPosession = int(boughtShares[0]["ShareSum"])
        else:
            sharesInPosession =int(boughtShares[0]["ShareSum"]) - int(soldShares[0]["ShareSum"])
        if shares > sharesInPosession:
            #return apology(str(shares) + str(sharesInPosession)+tableUser)
            return apology("you don't have as much shares to sell of this company in your portflio. examine your portfolio and inpiut another number of shares.")
        now = datetime.now()
        current_time = now.strftime("%D %H:%M:%S")
        #update the remaining cash in hand
        salePrice = quote["price"]
        available_balance= db.execute("SELECT cash from users WHERE id = ?",user_id)
        db.execute("UPDATE users SET cash = ? WHERE id = ?", available_balance[0]["cash"] +salePrice *shares, user_id)

        #record the purchase, log the stock, time of purchase, number of shares and the price of acquisition of the shares.
        db.execute("INSERT INTO ?(id, symbol, shares, price, type, time) VALUES(?,?,?,?,'sell',?)", tableUser, user_id, symbol,shares, salePrice ,current_time)
        return redirect("/")
    else :
        user_id= session["user_id"]
        user_tableName = db.execute("Select username FROM users where id = ?", user_id)
        # Retrieveing the name of the buy/sell log table of the user.
        tableUser = user_tableName[0]["username"]

        #values = db.execute("SELECT * FROM ? Where symbol = ?", tableUser ,symbol )
        options= []
        boughtShares = db.execute("SELECT symbol, SUM(shares) as ShareSum FROM ? where type = 'buy' GROUP BY symbol ",user_tableName[0]["username"])
        soldShares = db.execute("SELECT symbol, SUM(shares) as ShareSum FROM ? where type = 'sell' GROUP BY symbol ",user_tableName[0]["username"])
        if not boughtShares:
            return render_template("sell.html", options =options )
        elif not soldShares :
            for share in boughtShares:
                options.append(share["symbol"])
        else:
            for share in boughtShares:
                try:
                    soldSharesNumber =  next(item for item in soldShares if item["symbol"] == share["symbol"])
                except StopIteration:
                    soldSharesNumber = None
                #return apology(soldSharesNumber["symbol"] + str(soldSharesNumber["ShareSum"]))
                if not soldSharesNumber:
                    options.append(share["symbol"])
                elif share["ShareSum"] > soldSharesNumber["ShareSum"]:
                    options.append(share["symbol"])
        return render_template("sell.html",options =options )