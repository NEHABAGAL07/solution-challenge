// Wait for the DOM to be fully loaded
document.addEventListener('DOMContentLoaded', () => {

    // Splash Screen Logic
    setTimeout(() => {
        document.getElementById('splash-screen').style.display = 'none';
        document.getElementById('acc').style.display = 'block';
    }, 2000); // 2 seconds delay for splash screen

    // Onboarding Navigation
    document.getElementById('next-to-signup-screen').addEventListener('click', () => {
        document.getElementById('acc').style.display = 'none';
        document.getElementById('signup-screen').style.display = 'block';
    });

    document.getElementById('next-to-login-screen').addEventListener('click', () => {
        document.getElementById('acc').style.display = 'none';
        document.getElementById('login-screen').style.display = 'block';
    });

    // Back Navigation
    document.getElementById('back-to-splash-screen').addEventListener('click', () => {
        document.getElementById('acc').style.display = 'none';
        document.getElementById('splash-screen').style.display = 'block';
    });

    document.getElementById('back-to-acc').addEventListener('click', () => {
        document.getElementById('login-screen').style.display = 'none';
        document.getElementById('acc').style.display = 'block';
    });

    document.getElementById('back-to-onboarding-3').addEventListener('click', () => {
        document.getElementById('signup-screen').style.display = 'none';
        document.getElementById('acc').style.display = 'block';
    });

    // Authentication Logic
    document.getElementById('signup-btn').addEventListener('click', () => {
        alert("Sign up successful!");
        document.getElementById('signup-screen').style.display = 'none';
        document.getElementById('home-screen').style.display = 'block';
    });

    document.getElementById('login-btn').addEventListener('click', () => {
        alert("Login successful!");
        document.getElementById('login-screen').style.display = 'none';
        document.getElementById('home-screen').style.display = 'block';
    });

    // Sign Up Link in Login Screen
    document.getElementById('signup-link').addEventListener('click', () => {
        document.getElementById('login-screen').style.display = 'none';
        document.getElementById('signup-screen').style.display = 'block';
    });
});

/// Wait for the DOM to be fully loaded
document.addEventListener('DOMContentLoaded', () => {
    const gridItems = document.querySelectorAll('.grid-item');
    const middleSection = document.getElementById('middle-section');
    const dynamicContent = document.getElementById('dynamic-content');
    const pageContent = document.getElementById('page-content');
    const backToGrid = document.getElementById('back-to-grid');

    gridItems.forEach(item => {
        item.addEventListener('click', () => {
            const page = item.getAttribute('data-page');
            loadPageContent(page);
        });
    });

    backToGrid.addEventListener('click', () => {
        dynamicContent.style.display = 'none';
        middleSection.style.display = 'block';
    });

    function loadPageContent(page) {
        middleSection.style.display = 'none';
        dynamicContent.style.display = 'block';

        switch (page) {
            case 'appointments':
                pageContent.innerHTML = '<h2>Appointments</h2><p>Content for Appointments page.</p>';
                break;
            case 'womens-health':
                pageContent.innerHTML = '<h2>Women\'s Health</h2><p>Content for Women\'s Health page.</p>';
                break;
            case 'health-tracker':
                pageContent.innerHTML = '<h2>Health Tracker</h2><p>Content for Health Tracker page.</p>';
                break;
            case 'child-section':
                pageContent.innerHTML = '<h2>Child Section</h2><p>Content for Child Section page.</p>';
                break;
            case 'nearby-clinics':
                pageContent.innerHTML = '<h2>Nearby Clinics</h2><p>Content for Nearby Clinics page.</p>';
                break;
            case 'nutrition':
                pageContent.innerHTML = '<h2>Nutrition</h2><p>Content for Nutrition page.</p>';
                break;
            case 'mental-health':
                pageContent.innerHTML = '<h2>Mental Health</h2><p>Content for Mental Health page.</p>';
                break;
            case 'fitness':
                pageContent.innerHTML = '<h2>Fitness</h2><p>Content for Fitness page.</p>';
                break;
            case 'emergency-contacts':
                pageContent.innerHTML = '<h2>Emergency Contacts</h2><p>Content for Emergency Contacts page.</p>';
                break;
            default:
                pageContent.innerHTML = '<h2>Page Not Found</h2><p>The requested page could not be found.</p>';
                break;
        }
    }
});