const express = require('express');
const router = express.Router();
const userController = require('../controllers/userController');
const { validate, validateAdmin } = require('../controllers/loginController');

// GET /api/users/public/:username - Get public user profile by username (no auth required)
router.get('/public/:username', userController.getPublicUserProfile);

// GET /api/users - Get all users (admin only)
router.get('/', validate, userController.getAllUsers);

// GET /api/users/:id - Get user profile (requires authentication)
router.get('/:id', validate, userController.getUserProfile);

// PUT /api/users/:id - Update user profile (requires authentication)
router.put('/:id', validate, userController.updateUserProfile);

// POST /api/users/:id/change-password - Change password (requires authentication)
router.post('/:id/change-password', validate, userController.changePassword);

// DELETE /api/users/:id - Delete user and all their posts (admin only)
router.delete('/:id', validate, userController.deleteUser);

module.exports = router;
