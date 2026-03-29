#include <wx/wx.h>
#include <fstream>
#include <string>

const std::string USER_FILE = "users.txt";

// ─── Helper Functions ─────────────────────────────────────────────
bool userExists(const std::string& username) {
    std::ifstream file(USER_FILE);
    std::string line;
    while (std::getline(file, line)) {
        std::string storedUser = line.substr(0, line.find(':'));
        if (storedUser == username) return true;
    }
    return false;
}

bool registerUser(const std::string& username, const std::string& password) {
    if (userExists(username)) return false;
    std::ofstream file(USER_FILE, std::ios::app);
    file << username << ":" << password << "\n";
    return true;
}

bool loginUser(const std::string& username, const std::string& password) {
    std::ifstream file(USER_FILE);
    std::string line;
    while (std::getline(file, line)) {
        std::string storedUser = line.substr(0, line.find(':'));
        std::string storedPass = line.substr(line.find(':') + 1);
        if (storedUser == username && storedPass == password) return true;
    }
    return false;
}

// ─── Register Frame ───────────────────────────────────────────────
class RegisterFrame : public wxFrame {
public:
    RegisterFrame(wxWindow* parent) : wxFrame(parent, wxID_ANY, "Register", wxDefaultPosition, wxSize(400, 350)) {
        wxPanel* panel = new wxPanel(this);
        wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

        sizer->Add(new wxStaticText(panel, wxID_ANY, "Create an Account"), 0, wxALIGN_CENTER | wxTOP, 20);

        sizer->Add(new wxStaticText(panel, wxID_ANY, "Username:"), 0, wxLEFT | wxTOP, 20);
        txtUser = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxSize(340, 30));
        sizer->Add(txtUser, 0, wxLEFT | wxRIGHT, 20);

        sizer->Add(new wxStaticText(panel, wxID_ANY, "Password:"), 0, wxLEFT | wxTOP, 10);
        txtPass = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxSize(340, 30), wxTE_PASSWORD);
        sizer->Add(txtPass, 0, wxLEFT | wxRIGHT, 20);

        sizer->Add(new wxStaticText(panel, wxID_ANY, "Confirm Password:"), 0, wxLEFT | wxTOP, 10);
        txtConfirm = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxSize(340, 30), wxTE_PASSWORD);
        sizer->Add(txtConfirm, 0, wxLEFT | wxRIGHT, 20);

        wxButton* btnRegister = new wxButton(panel, wxID_ANY, "Register", wxDefaultPosition, wxSize(340, 35));
        sizer->Add(btnRegister, 0, wxLEFT | wxRIGHT | wxTOP, 20);

        panel->SetSizer(sizer);
        Centre();

        btnRegister->Bind(wxEVT_BUTTON, &RegisterFrame::OnRegister, this);
    }

private:
    wxTextCtrl* txtUser;
    wxTextCtrl* txtPass;
    wxTextCtrl* txtConfirm;

    void OnRegister(wxCommandEvent&) {
        std::string username = txtUser->GetValue().ToStdString();
        std::string password = txtPass->GetValue().ToStdString();
        std::string confirm  = txtConfirm->GetValue().ToStdString();

        if (username.empty() || password.empty() || confirm.empty()) {
            wxMessageBox("Please fill in all fields.", "Error", wxOK | wxICON_ERROR);
            return;
        }
        if (password != confirm) {
            wxMessageBox("Passwords do not match.", "Error", wxOK | wxICON_ERROR);
            return;
        }
        if (registerUser(username, password)) {
            wxMessageBox("Registered successfully! You can now log in.", "Success", wxOK | wxICON_INFORMATION);
            Close();
        } else {
            wxMessageBox("Username already exists. Choose another.", "Error", wxOK | wxICON_ERROR);
        }
    }
};

// ─── Login Frame ──────────────────────────────────────────────────
class LoginFrame : public wxFrame {
public:
    LoginFrame() : wxFrame(nullptr, wxID_ANY, "Login", wxDefaultPosition, wxSize(400, 300)) {
        wxPanel* panel = new wxPanel(this);
        wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

        sizer->Add(new wxStaticText(panel, wxID_ANY, "Welcome! Please Log In"), 0, wxALIGN_CENTER | wxTOP, 20);

        sizer->Add(new wxStaticText(panel, wxID_ANY, "Username:"), 0, wxLEFT | wxTOP, 20);
        txtUser = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxSize(340, 30));
        sizer->Add(txtUser, 0, wxLEFT | wxRIGHT, 20);

        sizer->Add(new wxStaticText(panel, wxID_ANY, "Password:"), 0, wxLEFT | wxTOP, 10);
        txtPass = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxSize(340, 30), wxTE_PASSWORD);
        sizer->Add(txtPass, 0, wxLEFT | wxRIGHT, 20);

        wxButton* btnLogin    = new wxButton(panel, wxID_ANY, "Login",    wxDefaultPosition, wxSize(165, 35));
        wxButton* btnRegister = new wxButton(panel, wxID_ANY, "Register", wxDefaultPosition, wxSize(165, 35));

        wxBoxSizer* btnSizer = new wxBoxSizer(wxHORIZONTAL);
        btnSizer->Add(btnLogin,    0, wxLEFT, 20);
        btnSizer->Add(btnRegister, 0, wxLEFT, 10);
        sizer->Add(btnSizer, 0, wxTOP, 15);

        panel->SetSizer(sizer);
        Centre();

        btnLogin->Bind(wxEVT_BUTTON,    &LoginFrame::OnLogin,    this);
        btnRegister->Bind(wxEVT_BUTTON, &LoginFrame::OnRegister, this);
    }

private:
    wxTextCtrl* txtUser;
    wxTextCtrl* txtPass;

    void OnLogin(wxCommandEvent&) {
        std::string username = txtUser->GetValue().ToStdString();
        std::string password = txtPass->GetValue().ToStdString();

        if (username.empty() || password.empty()) {
            wxMessageBox("Please enter username and password.", "Error", wxOK | wxICON_ERROR);
            return;
        }
        if (loginUser(username, password)) {
            wxMessageBox("Login successful! Welcome, " + txtUser->GetValue() + "!", "Success", wxOK | wxICON_INFORMATION);
        } else {
            wxMessageBox("Invalid username or password.", "Error", wxOK | wxICON_ERROR);
        }
    }

    void OnRegister(wxCommandEvent&) {
        RegisterFrame* reg = new RegisterFrame(this);
        reg->Show();
    }
};

// ─── App ──────────────────────────────────────────────────────────
class MyApp : public wxApp {
public:
    virtual bool OnInit() {
        LoginFrame* frame = new LoginFrame();
        frame->Show(true);
        return true;
    }
};

wxIMPLEMENT_APP(MyApp);